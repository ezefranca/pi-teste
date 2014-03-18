/*
 * list.c - implementation of the integer list functions
 */
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

list_t* lst_new() {
    list_t *list;
    list = (list_t*) malloc(sizeof(list_t));
    list->first = NULL;
    return list;
}
void lst_destroy(list_t *list) {
    //printf("lst_destroy\n");
    
    lst_iitem_t *head, *temp;
    head = list->first;
    
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    free(list);
}
void lst_insert(list_t *list, par_ordenado *value) {
    //printf("lst_insert: x=%d, y=%d\n", value->x, value->y);
    
    lst_iitem_t *temp,*right;
    temp = (lst_iitem_t*)malloc(sizeof(lst_iitem_t));
    temp->ponto = value;
    temp->next = NULL;
    
    right=list->first;
    if(!right) {
        list->first = temp;
    } else {
        while(right->next != NULL) right = right->next;
        right->next = temp;
        right = temp;
        right->next = NULL;
    }
}
void lst_remove(list_t *list, par_ordenado *value) {
    //printf("lst_remove: x=%d, y=%d\n", value->x, value->y);
    
    lst_iitem_t *temp, *prev;
    temp = list->first;
    while(temp != NULL) {
        if(temp->ponto == value) {
            if(temp == list->first) {
                list->first = temp->next;
                free(temp);
                return;
            } else {
                prev->next = temp->next;
                free(temp);
                return;
            }
        } else {
            prev = temp;
            temp = temp->next;
        }
    }
}
void lst_print(list_t *list) {
    printf("lst_print\n");
    lst_iitem_t *temp = list->first;
    while(temp) {
        printf("Ponto: %d, %d; ", temp->ponto->x, temp->ponto->y);
        temp = temp->next;
    }
    printf("\n");
}

par_ordenado* ponto_new(int x, int y) {
    par_ordenado *ponto = malloc(sizeof(par_ordenado));
    ponto->x = x;
    ponto->y = y;
    return ponto;
}