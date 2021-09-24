#ifndef GROTA_SPRITES_H
#define GROTA_SPRITES_H
#include <stdbool.h>
#include "core/mathematics.h"
#include "graphics/2d/camera2d.h"


typedef struct _Sprite{
    bool visible;
    GLuint *texture_id;
    Color4f color;
    Vec2 texture_scale;
	Transform2D transform;
} Sprite;

#define SPRITE_NEW {1,0,COLOR_WHITE,VEC2_FILL_ONE,TRANSFORM2D_IDENTITY}


bool sprites_init();
void sprite_draw(Sprite *sprite,Camera2D *camera2d);
void sprites_destroy();


Sprite* sprite_alloc();
void sprite_destroy(Sprite *this);

#endif