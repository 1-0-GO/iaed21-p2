/*
 * File:  OtherFuncs.h
 * Author:  Francisco Martins 99068
 * Description: Useful auxilary functions for this second project 
 				and the defines for the main file
*/

#ifndef OTHERFUNCS_H
#define OTHERFUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Path.h"

/*ABSTRACTIONS*/
/*abstractions for the sorting algorithm*/
#define less(a, b) (strcmp(a,b) < 0)
#define exch(a, b) {Item t = a; a = b; b = t;}
#define greater(a, b) (strcmp(a,b) > 0)
typedef char* Item_sort;		

/*STRINGS*/
/*Help Menu*/
#define HELP_MENU \
"help: Imprime os comandos disponíveis.\n"\
"quit: Termina o programa.\n"\
"set: Adiciona ou modifica o valor a armazenar.\n"\
"print: Imprime todos os caminhos e valores.\n"\
"find: Imprime o valor armazenado.\n"\
"list: Lista todos os componentes imediatos de um sub-caminho.\n"\
"search: Procura o caminho dado um valor.\n"\
"delete: Apaga um caminho e todos os subcaminhos.\n"

/*Exit Formats*/
#define visit_for_print(p) printf("/%s %s\n", p->name, p->value)
#define visit_for_find(p) printf("%s\n", p->value)
#define visit_for_list(path_name) printf("%s\n", last_component(path_name))
#define visit_for_search(p) printf("/%s\n", p->name)

/*Error Messages*/
/*find command errors*/
#define FIND_ERRO_1 "not found\n"	  
#define FIND_ERRO_2 "no data\n"
/*list command errors*/
#define LIST_ERRO_1 "not found\n"
/*search command errors*/
#define SEARCH_ERRO_1 "not found\n"
/*delete command errors*/
#define DELETE_ERRO_1 "not found\n"
/*out of memory error*/
#define ERROR_NOMEMORY "No memory.\n"

/*Command Names*/
#define COMMAND_HELP "help"
#define COMMAND_QUIT "quit"
#define COMMAND_SET "set"
#define COMMAND_PRINT "print"
#define COMMAND_LIST "list"
#define COMMAND_SEARCH "search"
#define COMMAND_DELETE "delete"
#define COMMAND_FIND "find"

/*DEFINED SIZES*/
#define MAX_SIZE_INSTRUCTION 65536		/*added one to it for the new line character*/
#define INITIAL_SIZE_AUX_LIST 30		/*initial size of the auxilary array used to sort
										the items of the directory for list command*/
#define CUTOFF_INSERTION_SORT 15		/*searched various possible values for best optimization
										of quicksort and landed on this one*/
#define MAX_NUM_PARAMS 3		/*command set receives 3 parameters*/
#define HT_INITIAL_SIZE 4999		/*initial size of the hash table. Prime size that 
									allows for 2999 elements in the hash table before
									having to expand (expands at 60% ocupation)*/ 

/*FUNCTIONS*/
void *checkedmalloc(size_t size);
void *checkedrealloc(void *ptr, size_t size);
char *duplicate_string(const char *source);
void insertion_sort(Item_sort a[], int l, int r);

#endif