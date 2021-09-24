#include "gameobject.h"
#include "graphics/2d/sprites.h"
#include "physics/physics2d.h"
#include "stdlib.h"
GameObject *gameobject_alloc() {
    GameObject *this=calloc(1,sizeof(GameObject));
    this->sprite=sprite_alloc();
    this->body=body_alloc();
    this->body->parent=this;
    return this;
}
void gameobject_destroy(GameObject *this) {
    sprite_destroy(this->sprite);
    body_destroy(this->body);
    free(this);
}
void gameobject_update(GameObject *this) {
    this->sprite->transform.position=this->body->position;
}