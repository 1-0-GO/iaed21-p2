	/*
 * File:  DataStructures.h
 * Author:  Francisco Martins 99068
 * Description: Implementation of the data structures for this project:
 				.) Implementation of a hash table with linear probing to
 				find links more efficiently
 				.) Implementation of the hierarchy tree for paths as a 
 				binary tree which is equivalent to a n-ary tree 
*/

#ifndef DS_H
#define DS_H

#include "Path.h"

/*STRUCTURE OF THE DATA TYPES*/
/*struct tree_s implemented as a binary tree equivalent to a n-ary tree 
since from any node you can only access two other struct tree_s: 
.) a child, path1 is child of path2 if the imediate subpath of path2 is path1;
   ex: /usr/local/lib is child of usr/local
.) a sibling, two paths are siblings if they have the same imediate subpath
   ex: /usr/local/lib and usr/local/bin are siblings*/ 
typedef struct treenode{	
	Path path;		/*the object we store in the tree is of type Path as defined in "Path.h"*/
	struct treenode *left_child;	/*a child, as defined above, of the current path */
 	struct treenode *right_sibling;	/*a sibling, as defined above, of the current path*/
 	} *link;

/*structure that represents a hash table, to search more efficiently for link type data*/	
typedef struct HashT_s{
	link *link_table;	/*table of links, allocated dynamically*/
	int M, N;	/*maximum size of the hash table and current number of elements, respectively*/
} *HashT;

/*FUNCTIONS FOR THE ADTs*/
/*Hash table functions*/
void HASHTinit(HashT *h_ref, int M);
void HASHTdestroy(HashT *h_ref);
void HASHTinsert(HashT h, link t);
link HASHTsearch(HashT h, char *path_name);

/*Binary tree functions*/
void TREEinit(link *head);
Path TREEsearch(link head, char *val);
void TREEtraverse_postorder(link head);
void TREEvisit_directory(link head, char ***a, int *current_size, int max_size);

/*Hybrid functions*/
void STinsert(link *head, HashT ht, Path p);
void STdestroy(link *head, HashT ht);
void STdelete(char *path_name, link *head, HashT ht);
void STinsert_allsubpaths(char *path_name, link *t_head,  HashT ht);

#endif	