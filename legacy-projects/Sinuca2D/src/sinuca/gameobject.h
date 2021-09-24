#ifndef SINUCA_GAMEOBJECT_H
#define SINUCA_GAMEOBJECT_H
#include "graphics/2d/sprites.h"
#include "physics/physics2d.h"
typedef struct _GameObject {
    Sprite *sprite;
    Body *body;
} GameObject;
GameObject *gameobject_alloc();
void gameobject_destroy(GameObject *this);
void gameobject_update(GameObject *this);
#endif