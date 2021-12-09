/*
 * File:  Path.c
 * Author:  Francisco Martins 99068
 * Description: Definition of the object path that's to 
 				be manipulated and its related functions
*/

#include "Path.h"

/*Henceforth, the standard form of the name of a path means that the name has no
slashes in the beginning or the end and no extra slashes between its components*/

/*returns a new path name, allocated dynamically, that corresponds to the name
of the subpath of path_name that has only n components*/
char *firstncomponents(char *path_name, int n){
	int i;
	Item abridged_name = checkedmalloc(sizeof(char)*(strlen(path_name)+1));
	/*copies path_name (which is in standard format) from its beginning until n-1 
	slashes are read or it reaches the end of path_name*/
	for (i = 0; n >= 1 && path_name[i] != '\0'; i++){
		if (path_name[i] == '/')
			n--;
		abridged_name[i] = path_name[i];
	}
	/*if it didn't reach the end of path_name than it has an extra character '/'*/ 
	if (path_name[i] != '\0')	
		i--;
	abridged_name[i] = '\0';	/*terminate the string with '\0'*/
	return abridged_name;
}
	
/*counts the number of components of path_name*/	 
int countcomponents(char *path_name){
	/*counts the number of components of path_name (which is in standard format) so 
	count starts at 1 (path_name mustn't be empty) and then each new component means
	an additional slash so we just count the number of slashes*/
	int count = 1;
	for (; *path_name != '\0'; path_name++)
		if (*path_name == '/')
			count++;
	return count;	
}

/*function to print the last component of a path_name in standard form*/
char *last_component(char *path_name){
	/*start at the end of path_name (excluding '\0') and walk backwards from there*/
	char *begin = path_name + strlen(path_name) - 1;
	/*when the beginning of the path_name (which is in standard form) is reached or 
	a slash is reached then the beggining-1 of last component is obtained*/
	while (begin>=path_name && *begin != '/')
		begin--;
	/*an additional position must be added due to the way the while loop is done*/
	return ++begin;
}


/*in-place function that returns the trimmed string that corresponds to the std_name
as defined in the beggining of this file. Slashes are trimmed from the beginning and
end of the string*/
char *std_name(char *path_name){
	/*start at the end of path_name (excluding '\0') */
	int end = strlen(path_name)-1;
	/*walk from the end of path_name while there are still slashes in the end of the
	string to cut/trim/nullify*/
	while (end > 0 && path_name[end] == '/')
		path_name[end--] = '\0';
	/*walk from the beginning of path_name while there are still slashes in the end of the
	string to cut/trim/nullify*/
	while (*path_name != '\0' && *path_name == '/')
		path_name++;
	/*return the new beginning of the string which now has no slashes in the end or beginning*/ 
	return path_name;
}

/*function to create a new Path type item*/
Path newPath(char *name, Item value){
	/*the new item is allocated dynamically, as are both its components using duplicate_string*/
	Path p = checkedmalloc(sizeof(struct Path_s));
	p->name = duplicate_string(name);
	/*the value might not be set right away*/
	if (value != NULLitem)
		p->value = Duplicate(value);
	else 
		p->value = NULLitem;
	return p;
}

/*function to delete an item of type path*/
void deletePath(Path p){
	/*free the path's components and then free the path*/
	free(p->name);
	FREEitem(p->value);
	free(p);
}	