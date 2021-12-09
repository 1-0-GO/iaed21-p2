/*
 * File:  DataStructures.c
 * Author:  Francisco Martins 99068
 * Description: Implementation of the data structures for this project:
 				.) Implementation of a hash table with linear probing to 
 				find links more efficiently
 				.) Implementation of the hierarchy tree for paths as a 
 				binary tree but it is equivalent to a n-ary tree 
*/

#include "DataStructures.h"


/*hash function for strings*/
int hash(char *v, int M){
	unsigned h, a = 31415, b = 27183;
	for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
		h = (a*h + *v) % M;
	return h;
}

/*function that initializes the parameters of a hash table*/
void HASHTinitialize_values(HashT h, int M){
	int i; 
	h->M = M;
	h->N = 0;
	h->link_table = checkedmalloc(M*sizeof(link));
	for (i = 0; i < M; i++)
		h->link_table[i] = NULL; 	/*initialize everything to NULL*/
}

/*function that initializes a hash table*/
void HASHTinit(HashT *h_ref, int M) {
	*h_ref = checkedmalloc(sizeof(struct HashT_s));
	HASHTinitialize_values(*h_ref, M);	
}

/*function to free all memory associated with a hash table*/
void HASHTdestroy(HashT *h_ref){
	free((*h_ref)->link_table);
	free(*h_ref);
	*h_ref = NULL;
}

/*auxilary function that doubles the size of the link table associated 
with a hash table*/
void HASHTexpand(HashT h) {
	int i, M = h->M;
	link *t = h->link_table;	/*store the previous link table*/	
	/*change the link table associated with the hash table to an empty one with double 
	the size and update the components of the hash table -> M to 2*M and N to 0*/
	HASHTinitialize_values(h, M+M);		
	/*copy the items from the previous link table to the new link table an update N*/
	for (i = 0; i < M; i++)
		if (t[i] != NULL)
			HASHTinsert(h, t[i]);
	free(t);	/*free the old table*/
}

/*function to insert a new link in the hash table (this hash table 
has resolution by linear probing)*/
void HASHTinsert(HashT h, link t) {
	int i = hash(t->path->name, h->M);
	while (h->link_table[i] != NULL)	/*there might be collisions*/
		i = (i+1) % (h->M);		/*linear probing method of handling with them*/
	h->link_table[i] = t;
	/*if the capacity exceeds 60% the hash table is expanded*/
	if ((h->N)++ > 0.6*(h->M))	
		HASHTexpand(h);
}	

/*function to search for a link in the hash table (this hash table 
has resolution by linear probing)*/
link HASHTsearch(HashT h, char *path_name){
	int i = hash(path_name, h->M);
	while (h->link_table[i] != NULL){
		if (!strcmp(h->link_table[i]->path->name, path_name))
			return h->link_table[i];
		else
			i = (i+1) % h->M;
	}	
	return NULL;
}

/*function to search for a link in the hash table (this hash table 
has resolution by linear probing)*/
void HASHTdelete(HashT h, char *path_name) { 
	int i = hash(path_name, h->M), j;
    link t;
    while (h->link_table[i] != NULL){
	    if (!strcmp(h->link_table[i]->path->name, path_name))
 	       break;
        else
			i = (i+1) % (h->M);
    }      
    if (h->link_table[i] == NULL)	return;	/*the element doesn't exist*/
	h->link_table[i] = NULL;	/*set the element to delete to NULL*/
	h->N--;		/*update the number of elements*/
    /*reinsert all previous items until a new empty postion is found*/	
  	for (j = (i+1) % (h->M); h->link_table[j] != NULL; j = (j+1) % h->M) {
    	t = h->link_table[j]; 
    	h->link_table[j] = NULL;
    	h->N--;
    	HASHTinsert(h, t);
    }
}


/*function to initializes a link type object*/
void TREEinit(link *head){
	*head = NULL;
}

/*function to create a new link type object*/
link new_link(Path path, link lc, link rs) {
	link t = checkedmalloc(sizeof(struct treenode));
	t->path = path; 
	t->left_child = lc;
	t->right_sibling = rs;
	return t;
}

/*auxilary function to search for a Path by value in the binary 
tree indicated by this functions first parameter*/
Path searchR(link head, char *val){
	Path path;
	if (head == NULL)	/*the tree is empty*/
		return NULL;
	/*the path was found*/	
	if (head->path->value != NULLitem && eq(val, head->path->value))	
		return head->path;	
	/*search, recursively, the left tree for the path*/
	path = searchR(head->left_child, val);
	/*if some path was found in the left tree (NULL means it wasn't 
	found) it returns the path*/
	if (path != NULL)
		return path;
	/*if not the right tree is searched, recursively. If it doesn't 
	exist there it'll return NULL*/
	return searchR(head->right_sibling, val);
}

/*main function to search for a Path by value in the binary 
tree indicated by this functions first parameter*/
Path TREEsearch(link head, char *val){
	return searchR(head, val);
}

/*recursive function for pre-order traversal of a tree that prints
its elements using the visit_for_print function*/
void TREEtraverse_postorder(link head){
	if (head == NULL)	/*nothing else to print on this side*/
		return;
	/*print, recursively, the left and then right subtree*/
	TREEtraverse_postorder(head->right_sibling);
	TREEtraverse_postorder(head->left_child);
	if(head->path->value != NULLitem)	
		visit_for_print(head->path);
}
	
/*function that stores all the path names in the same directory as head in the 'a' 
dynamic array in its second paramer. It changes current_size to the 
number of elements that were put, sequentially, in 'a'*/
void TREEvisit_directory(link head, char ***a, int *current_size, int max_size){
	char **array = *a;
	while (head != NULL){
		if (*current_size >= max_size-1){	/*the array must be resized*/
			max_size = max_size*2;
			*a = checkedrealloc(*a, sizeof(char*)*max_size);
			array = *a;
		}
		/*the path names are stored sequentially in 'a'*/
		array[(*current_size)++] = head->path->name;
		/*access next element in the directory*/
		head = head->right_sibling;
	}	
}

/*auxilary function to insert a new link in the tree*/
void insertR(link *link_ref, HashT ht, Path p){
	*link_ref = new_link(p, NULL, NULL);
	HASHTinsert(ht, *link_ref);
}

/*main function to insert a new link in the tree*/
void STinsert(link *head_ref, HashT ht, Path p){
	int n = countcomponents(p->name); 
	link prev, curr;
	if (*head_ref == NULL){		/*the tree is empty*/
		insertR(head_ref, ht, p);
		return;
	}
	if  (n==1){ 		/*the element to insert is in the 'root'*/
		curr = *head_ref;
	
	}	
	else{		
		/*get the name of the parent directory of path*/
		char *parent_path_name = firstncomponents(p->name, n-1);
		/*get the link for the parent directory path*/
		prev = HASHTsearch(ht, parent_path_name);
		curr = prev->left_child;	/*go to its first child*/
		free(parent_path_name);
		if (curr==NULL){	/*if the parent directory had no children*/
			insertR(&(prev->left_child), ht, p);
			return;
		}
	}	
	/*go through the directory until its end is reached*/
	for (prev = NULL; curr!= NULL; prev=curr, curr=curr->right_sibling)
	;	
	/*insert in the end*/
	insertR(&(prev->right_sibling), ht, p);
}

/*function to free all memory associated with a link and its components (its path)*/
void free_link(link head){
	deletePath(head->path);
	free(head);
}

/*auxilary function to delete a link from a tree*/
void destroyR(link head, HashT ht){
	/*it's basically a post-order traversal 
	to delete all paths in the tree (or subtree)*/ 
	if (head == NULL)
		return;
	destroyR(head->left_child, ht);
	destroyR(head->right_sibling, ht);
	if (ht != NULL)	/*if the hash table has already been destroyed because the delete 
					command with no arguments was called deletion from the hash table is
					unnecessary and acctually wrong*/
		HASHTdelete(ht, head->path->name);	/*delete the link from the hash table*/
	free_link(head);	/*destroy/free the link*/
}

/*main function to delete a link from a tree*/
void STdestroy(link *head, HashT ht){
	destroyR(*head, ht);
	*head = NULL;	/*set the head to NULL to avoid problems further*/
}

/*auxilary function to delete delete_link, the functions first parameter, from 
the tree and the hash table.*/
void deleteR(link *delete_link, HashT ht){
	link aux = *delete_link;
	*delete_link = aux->right_sibling;
	STdestroy(&(aux->left_child), ht); 
	HASHTdelete(ht, aux->path->name);
	free_link(aux);
}

/*main function to delete a link from a tree and hash table*/
void STdelete(char *path_name, link *head, HashT ht){
	int n = countcomponents(path_name); 
	link prev, curr;
	if  (n==1){ 		/*the element to delete is in the 'root'*/
		curr = *head;
		if (!strcmp(curr->path->name, path_name)){ 		/*if the head is to be deleted*/
			deleteR(head, ht);
			return;
		}	
	}	
	else{		
		/*get the name of the parent directory of path*/
		char *parent_path_name = firstncomponents(path_name, n-1);
		/*get the link for the parent directory path*/
		prev = HASHTsearch(ht, parent_path_name);
		curr = prev->left_child;	/*go to its first child*/
		free(parent_path_name);
		if (!strcmp(curr->path->name, path_name)){		/*the first child is the element to delete*/
			deleteR(&(prev->left_child), ht);
			return;
		}
	}	
	/*deletion similar to the one made in a simply linked list*/
	for (prev = NULL; curr!= NULL; prev=curr, curr=curr->right_sibling){
		if (!strcmp(curr->path->name, path_name)){
			deleteR(&(prev->right_sibling), ht);
			break;
		}
	} 
}

/*function to insert in the tree and hash table the links corresponding 
to the subpaths of path_name*/
void STinsert_allsubpaths(char *path_name, link *head, HashT ht){	
	link t;
	int i, n = countcomponents(path_name);
	char *sub_path_name;
	Path path;	
	for (i = 1; i<n; i++){
		/*insert every link subpath associated with path_name, if it 
		doesn't exist already, from the subpath with 1 components up to, 
		but not including, the subpath that has n-1 components. They are 
		set with null value and the name with i components that is subpath
		of path_name*/
		sub_path_name = firstncomponents(path_name, i);
		if ((t=HASHTsearch(ht, sub_path_name)) == NULL){
			path = newPath(sub_path_name, NULLitem);
			STinsert(head, ht, path);
			free(sub_path_name);		
		}
		else
			free(sub_path_name);
	}						
}
