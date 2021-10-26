#ifndef DENISE_ECS_H
#define DENISE_ECS_H
#include "core/structs.h"
#include "core/dmath.h"

typedef struct _Entity Entity;
struct _Entity{
    Transform3D *transform;
    List* components;
    /* List of pointers to entities*/
    List* childs;
};

typedef enum  _ComponentType{
    RendererComponent
} ComponentType;

typedef struct _Component{
    ComponentType type;
    void *pointer;
} Component;


#endif