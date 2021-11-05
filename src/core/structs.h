#ifndef DENISE_STRUCTS_H
#define DENISE_STRUCTS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _List List;
struct _List {
    void *pointer;
    List *prev;
    List *next;
};

static const List LIST_NEW = {0,0,0};


static inline List* list_iterate(List *node) {
    if(node) return node->next;
    return NULL;
}

static inline List* list_add(List *node,void *pointer) {
    List *new_node=(List *)calloc(1,sizeof(List));
    new_node->pointer=pointer;
    if(node) {
        new_node->next=node;
        new_node->prev=node->prev;
        node->prev=new_node;
    }
    return new_node;
}

static inline List* list_remove(List *node,void *pointer) {
   if(node->pointer==pointer) {
       List *ret=NULL;
        if(node->prev) {
            ret=node->prev;
            node->prev->next=node->next;
        }
        if(node->next){
            ret=node->next;
            node->next->prev=node->prev;
        }
        free(node);
        return ret;       
   }
   List *first_node=node;
   while(first_node->prev)
    first_node=first_node->prev;

    if(first_node->pointer==pointer)
     return list_remove(first_node,pointer);

    bool found_pointer=false;
    List *next_node=first_node->next;
    while(next_node->next) {
        if(next_node->pointer==pointer) {
            next_node=list_remove(next_node,next_node->pointer);
            break;
        }
        next_node=next_node->next;
    }

    if(!found_pointer) {
        printf("i cant found the pointer that i tring to remove from list\n");
    }
    return node;
}

static inline List * list_destroy(List *node) {
    List* next_node=node;
    while(next_node) next_node=list_remove(next_node,next_node->pointer);
    return NULL;
}

#endif