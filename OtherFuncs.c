/*
 * File: OtherFuncs.c
 * Author:  Francisco Martins 99068
 * Description: Useful auxilary functions for this second project
*/
#include "OtherFuncs.h"

/*function to check the return value of malloc. If the memory runs out, the program
terminates and prints some 'out of memory' error to stdin*/
void *checkedmalloc(size_t size){
	void *aux = malloc(size);	/*try to allocate the memory using malloc*/
	if (aux == NULL){	
		/*if the malloc is unsuccessful the program prints ERROR_NOMEMORY
		 to stdin and leaves*/
		printf("%s\n", ERROR_NOMEMORY);
		exit(0);
	}	
	/*else it returns the value of the pointer returned by malloc*/
	return aux;		
}

/*function to check the return value of realloc. If the memory runs out, the program
terminates and prints some out of memory error to stdin*/
void *checkedrealloc(void *ptr, size_t size){
	void *aux = realloc(ptr, size);		/*try to allocate the memory using realloc*/
	if (aux == NULL){
		/*if the realloc is unsuccessful the program prints ERROR_NOMEMORY
		 to stdin and leaves*/
		printf("%s\n", ERROR_NOMEMORY);		
		exit(0);
	}	
	/*else it returns the value of the pointer returned by realloc*/
	return aux;	
}

/*similar to strdup from POSIX. It duplicates using malloc the source string to a new string*/
char *duplicate_string(const char *source){
	char *destination = checkedmalloc(sizeof(char)*(strlen(source)+1));
	return strcpy(destination, source);				
}

/*function to sort the items of 'a' using the insertion sort algorithm*/
void insertion_sort(Item_sort a[], int l, int r){
	int i, j;
	for (i = l+1; i <= r; i++) {
		Item_sort v = a[i];
		j = i-1;
		while ( j>=l && greater(v, a[j])) {
			a[j+1] = a[j];
			j--;
		}
	a[j+1] = v;
	}
}