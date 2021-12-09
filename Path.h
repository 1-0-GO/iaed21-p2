/*
 * File:  Path.h
 * Author:  Francisco Martins 99068
 * Description: Definition of the object path that's to 
 				be manipulated and its related functions 
*/

#ifndef PATH_H
#define PATH_H

#include "OtherFuncs.h"

/*ABSTRACTIONS*/
/*abstraction for Item value of Path*/
#define eq(item1, item2) (strcmp(item1, item2) == 0)
#define Duplicate(item) duplicate_string(item)
#define NULLitem NULL
#define FREEitem(item) free(item)
typedef char* Item;		/*abstraction for the type of value stored in path*/

/*PATH STRUCT*/
typedef struct Path_s{
	char *name;
	Item value;
} *Path;

/*PATH FUNCTIONS*/
char *firstncomponents(char *path_name, int n);
int countcomponents(char *path_name);
char *last_component(char *path_name);
char *std_name(char *path_name);
Path newPath(char *name, Item value);
void deletePath(Path p);

#endif