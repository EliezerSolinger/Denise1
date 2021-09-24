#ifndef GROTA_DLIST_H
#define GROTA_DLIST_H
#include "introspection.h"

#define CAST(pointer,type) ((*type)pointer)

typedef struct List List;
struct List {
    unsigned int type;
    void *pointer;
    List *prev;
    List *next;
};

#define LIST_NEW {0,0,0,0}


static inline List * list_iterate(List *node) {
    if(node) return node->next;
    else return 0;
}
static inline unsigned int list_count(List *node) {
    unsigned int count=0;
    List* next_node=node;
    while(next_node) {
        count++;
        next_node=list_iterate(next_node);
    };
    return count;
}
List* list_add(List *node,void *pointer);
List* list_remove(List *node,void *pointer);
List* list_destroy(List *node);


#endif