#ifndef GROTA_BSP_H
#define GROTA_BSP_H

#include "core/mathematics.h"
#include "core/list.h"
#include "graphics/3d/renderer.h"
#include "graphics/3d/camera.h"

void rlist_draw(List *node,Camera *camera);

#define BSPNODE_RIGHT 1
#define BSPNODE_LEFT 2

typedef struct _BSPNode BSPNode;
struct _BSPNode {
    Vec3 size;
    Vec3 position;
    BSPNode *left;
    BSPNode *right;
    Renderer *renderer;
};

typedef struct _RenderScene {
    List *renderer_list;
    BSPNode *tree;
}RenderScene;

RenderScene* scn_alloc();
RenderScene* scn_destroy(RenderScene *scn);
void scn_draw(RenderScene *scn,Camera *camera);
void scn_add(RenderScene *scn,Renderer *renderer);
void scn_remove(RenderScene *scn,Renderer *renderer);


#define BSPNODE_NEW {VEC3_FILL_ONE,VEC3_ZERO,0,0,0}

void bsp_draw(BSPNode *node,Camera *camera);
void bsp_add(BSPNode *node,Renderer *renderer);
void bsp_remove(BSPNode *node,Renderer *renderer);
BSPNode * bsp_destroy(BSPNode *root);

List * rlist_optimize(List *node,BSPNode *root);
void scn_optimize(RenderScene *scn);
#endif