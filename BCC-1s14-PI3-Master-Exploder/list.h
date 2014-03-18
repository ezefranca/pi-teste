#ifndef __LIST_H__

#define __LIST_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} par_ordenado;

/* lst_iitem - each element of the list points to the next element */
typedef struct lst_iitem {
    par_ordenado *ponto;
    struct lst_iitem *next;
} lst_iitem_t;

/* list_t */
typedef struct {
    lst_iitem_t * first;
} list_t;

/* lst_new - allocates memory for list_t and initializes it */
list_t* lst_new();

/* lst_destroy - free memory of list_t and all its items */
void lst_destroy(list_t *);

/* lst_insert - insert a new item with value 'value' in list 'list' */
void lst_insert(list_t *list, par_ordenado *value);

/* lst_remove - remove first item of value 'value' from list 'list' */
void lst_remove(list_t *list, par_ordenado *value);

/* lst_print - print the content of list 'list' to standard output */
void lst_print(list_t *list);

par_ordenado* ponto_new(int x, int y);

#endif