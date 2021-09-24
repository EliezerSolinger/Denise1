#include "bola.h"
#include "stdlib.h"
#include "graphics/graphics.h"
#include <stdio.h>

Bola* bola_carregar(int num) {
    Bola *this=calloc(1,sizeof(Bola));
    this->object=gameobject_alloc();
    char path[30];
    sprintf(path,"bolas/%d.png",num);
    this->active=true;
    this->object->sprite->texture_id=load_texture(path,false);
    this->object->sprite->transform.size=vec2_create(16,16);
    this->object->body->position=this->object->sprite->transform.position;
    this->object->body->dynamic_friction=0.04f;
    this->object->body->type=BODY_SPHERE;
    this->object->body->bouncy=0.9f; 
    body_resize(this->object->body,this->object->sprite->transform.size,30.0f);
    return this;
}

void bola_atualizar(Bola *this) { 
    gameobject_update(this->object);
    if(vec2_magnitude(this->object->body->velocity)<5.f)
         body_stop(this->object->body);
}
void bola_ativar(Bola *this) {
    this->active=true;
    this->object->sprite->visible=true;
    this->object->body->active=true;
}
void bola_desativar(Bola *this) {
    this->active=false;
    this->object->sprite->visible=false;
    this->object->body->active=false;
}
void bola_destroy(Bola *this) {
    if(!this->object) return;
    
    if(this->object->sprite->texture_id) {
        texture_destroy(this->object->sprite->texture_id);
    };
   
    gameobject_destroy(this->object);
    free(this);
}