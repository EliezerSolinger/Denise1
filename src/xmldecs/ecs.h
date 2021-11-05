#ifndef DENISE_ECS_H
#define DENISE_ECS_H
#include "../core/structs.h"
#include "../core/dmath.h"

typedef struct _Entity Entity;
struct _Entity{
    Transform3D transform;
    List* renderers;
    List* components;
    /* List of pointers to entities child*/
    List* childs;
};

typedef enum  _ComponentType{
    RendererComponent,
    CameraComponent
} ComponentType;

typedef struct _Component{
    ComponentType type;
    void *pointer;
} Component;


static const Entity ENTITY_NEW = {
    TRANSFORM3D_IDENTITY,
    NULL,
    NULL
};

#endif