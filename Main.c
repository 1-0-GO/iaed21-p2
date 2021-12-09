/*
 * File:  Main.c
 * Author:  Francisco Martins 99068
 * Description: Main file for the implementation of a hierarchical 
 				storage system similar to a file system. The storage 
 				system associates paths with values.
*/


#include "OtherFuncs.h"
#include "DataStructures.h"

/*Function to obtain the input from the user from stdin.*/
int get_params(char **params, char *buf){
	int i;
	/*initialize the parameters stored*/
	for (i = 0; i < MAX_NUM_PARAMS; i++)
		params[i] = NULL;
	/*get the full line from stdin put it in buffer*/
	if (fgets(buf, MAX_SIZE_INSTRUCTION+1, stdin) == NULL)
		return 0;
	/*obtain the first parameter up to a space or \n 
	character, excluding them from the string*/
	params[0] = strtok(buf, " \n");
	/*command search is the only one different than the others because 
	its second argument is a value (so it may contain spaces)*/
	if (!strcmp(params[0], COMMAND_SEARCH))
		params[1] = strtok(NULL, "\n");
	/*for any other project this part of the code is reusable and broad enough 
	that it should work whenever reading from the terminal is necessary. Additionally,
	the separator of the parameters might be specified as an additional parameter of
	get_params to make it even broader*/
	else{	
		i = 1;
		/*obtain the first parameter up to a space or \n 
		character, excluding them from the string*/
		while(i < MAX_NUM_PARAMS-1 && (params[i] = strtok(NULL, " \n")) != NULL)
			i++;
		/*obtain the whole last parameter, spaces and all. It may be NULL, since strtok
		may return NULL*/
		if (i == MAX_NUM_PARAMS-1)	
			params[i] = strtok(NULL, "\n");
	}	
	return 1;
}

/*Function that initializes all data structures, given pointers to it.*/
void initialize(link *tree_ref, HashT *ht_ref){
	TREEinit(tree_ref);
	HASHTinit(ht_ref, HT_INITIAL_SIZE);
}	

/*Function that destroys the information in all the data structures, given 
pointers to it. All destroy function set the data structures pointer to NULL.*/
void destroy_everything(link *tree_ref, HashT *ht_ref){
	HASHTdestroy(ht_ref);
	STdestroy(tree_ref, *ht_ref);
}

/*Function for help command.*/	
void command_help(){
	printf("%s", HELP_MENU);
}

/*Function for set command.*/
void command_set(char **params, link *tree_ref, HashT ht){
	link t;
	/*in-place function that changes its input and ignores slashes in the 
	beginning and the end of the string path which is the second (1+1) 
	parameter given by the user (so we access params[1]). Henceforth, no more 
	comments regarding std_name will be made since for further reference one
	might look to the definition of std_name or this comment.*/
	char *path_name = std_name(params[1]);	
	/*value is the third (2+1) parameter given by the user*/
	Item value = params[2];
	/*if item wasn't already char* a conversion to the right type of value might be done here*/
	/*search for the name in the hash table*/
	if ((t = HASHTsearch(ht, path_name)) != NULL){	/*if it exists*/
		if (t->path->value != NULLitem)	
			/*if the value has already been set it must first be freed*/
			FREEitem(t->path->value);		
		t->path->value = Duplicate(value);	/*assign the new value to the path*/
	}
	else{	/*if it doesn't exist*/
		STinsert_allsubpaths(path_name, tree_ref, ht);	/*first all subpaths are inserted*/ 
		/*a new path is created with the specified name and value which is 
		then inserted in the tree and the hash table*/	
		STinsert(tree_ref, ht, newPath(path_name, value));
	}
}

/*function for print command*/
void command_print(link head){
    TREEtraverse_postorder(head);
}

/*function for find command*/
void command_find(char **params, HashT ht){
	char *path_name = std_name(params[1]);	/*for further information on std_name view line 66
											or the definition of the function. Note that path_name 
											is the second parameter given by the user and so is 
											in position 1 of params*/
	link t = HASHTsearch(ht, path_name);	/*search for the name in the hash table*/	
	if (t == NULL)	/*if the path doesn't exist*/
		printf(FIND_ERRO_1);
	else if (t->path->value == NULL)	/*if it exists but has no value associated to it*/
		printf(FIND_ERRO_2);
	else
		visit_for_find(t->path);
}

/*function for list command*/
void command_list(char **params, link head, HashT ht){
	/*allocate space for the auxilary dynamic array of path names that
	later, in this function, is going to be sorted alphabetically (ASCII order)*/
	char **a = checkedmalloc(sizeof(char*)*INITIAL_SIZE_AUX_LIST);						
	int current_size = 0, i;
	if (params[1] == NULL)	/*if the list command was called without arguments. Note that the verification 
							done this way is possible because get_params initializes all parameters to NULL*/
		/*visit_directory stores in 'a' all the path names in the same directory as head (so 
		it's going to store the paths in the 'root' directory) and changes current_size to the 
		number of elements that were put, sequentially, in 'a'*/
		TREEvisit_directory(head, &a, &current_size, INITIAL_SIZE_AUX_LIST);
	else{	/*if list was called with an argument*/
		char *path_name = std_name(params[1]);	/*for further information on std_name view line 66
												or the definition of the function. Note that path_name 
												is the second parameter given by the user and so is 
												in position 1 of params*/
		link directory = HASHTsearch(ht, path_name);	/*search for the name in the hash table*/
		if (directory == NULL)	/*if the path doesn't exist*/
			printf(LIST_ERRO_1);
		else
			/*visit_directory stores in 'a' all the path names in the same directory as directory->left_child 
			(so it's going to store the paths that are the imediate components of directory) and changes 
			current_size to the number of elements that were put, sequentially, in 'a'*/
			TREEvisit_directory(directory->left_child, &a, &current_size, INITIAL_SIZE_AUX_LIST);	
	}
	insertion_sort(a, 0, current_size-1);	/*sorts the path names in 'a' alphabetically (ASCII order)*/
	/*print all elements of a, that are now sorted alphabetically (ASCII order)*/
	for (i = 0; i < current_size; i++)
		visit_for_list(a[i]);
	free(a);	/*'a' was dinamically alocated*/
}

/*function for search command*/
void command_search(char **params, link head){
	char *value = params[1];	/*note that value is the second parameter given by the user and 
								so is in position 1 of params*/
	Path path = TREEsearch(head, value);	/*search by order of creation the path that 
											corresponds to the indicated value*/
	if (path == NULL)	/*if there's no path with the indicated value*/	
		printf(SEARCH_ERRO_1);
	else 
		visit_for_search(path);
}	

/*function for delete command*/
void command_delete(char **params, link *tree_ref, HashT *ht_ref){
	if (params[1] == NULL){		/*if list was called without arguments. Note that the verification done 
								this way is possible because get_params initializes all parameters to NULL*/
		destroy_everything(tree_ref, ht_ref);	/*all data assotiated with the hash table and 
												the tree destroy and their head are set to NULL*/
		HASHTinit(ht_ref, HT_INITIAL_SIZE);		/*obtain a new empty hash table*/
	}
	else{	/*if list was called with an argument*/
		char *path_name = std_name(params[1]);	/*for further information on std_name view line 66
												or the definition of the function. Note that path_name
												is the second parameter given by the user and so is 
												in position 1 of params*/
		if (HASHTsearch(*ht_ref, path_name) == NULL)	/*if there's no path with that name in the hash table*/
			printf(DELETE_ERRO_1);
		else
			STdelete(path_name, tree_ref, *ht_ref);
	}
}	


int main(){
	char *params[3];
	char buf[MAX_SIZE_INSTRUCTION+1];
	link hierarchy;
	HashT ht;
	initialize(&hierarchy, &ht);
	/*a loop that executes the command corresponding to the first parameter
	indicated by the user, it ends when command quit is detected or get_params
	returns 0 (which happens when fgets returns NULL)*/
	while(get_params(params, buf)){
		if(!strcmp(params[0], COMMAND_QUIT))
			break;	
		if (!strcmp(params[0], COMMAND_HELP))
			command_help();	
		else if (!strcmp(params[0], COMMAND_SET))
			command_set(params, &hierarchy, ht);
		else if (!strcmp(params[0], COMMAND_PRINT))
			command_print(hierarchy);
		else if (!strcmp(params[0], COMMAND_FIND))
			command_find(params, ht);
		else if (!strcmp(params[0], COMMAND_LIST))
			command_list(params, hierarchy, ht);
		else if (!strcmp(params[0], COMMAND_SEARCH))
			command_search(params, hierarchy);
		else if (!strcmp(params[0], COMMAND_DELETE))
			command_delete(params, &hierarchy, &ht);	
	}
	/*in the end clear all memory*/
	destroy_everything(&hierarchy, &ht);
	return 0;
}