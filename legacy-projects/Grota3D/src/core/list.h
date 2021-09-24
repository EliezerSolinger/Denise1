#ifndef GROTA_DLIST_H
#define GROTA_DLIST_H

typedef struct List List;
struct List {
    void *pointer;
    List *prev;
    List *next;
};

#define LIST_NEW {0,0,0}

List* list_iterate(List *node);
List* list_add(List *node,void *pointer);
List* list_remove(List *node,void *pointer);
List* list_destroy(List *node);


#endif