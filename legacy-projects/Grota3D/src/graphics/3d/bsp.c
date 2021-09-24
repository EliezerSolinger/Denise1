#include "bsp.h"
#include "core/mathematics.h"
#include "graphics/3d/camera.h"
#include "graphics/3d/renderer.h"
#include "graphics/graphics.h"
#include <stdlib.h>
#include <float.h>
#include <stdio.h>


static int deep_counter=0;
void bsp_debugdraw(Camera *camera,BSPNode *node) {
    Mat4 model=MAT4_IDENTITY;
    mat4_translate_vec3(&model,node->position);
    mat4_scale_vec3(&model,node->size);
    float deepvalue=1.0f/deep_counter;
   // printf("%d\n",deep_counter);
    debugbox_draw(camera,model,color4f_create(0,deepvalue,0,1));
}



void rlist_draw(List *node,Camera *camera) {
    if(!node) {
        printf("i cant draw null pointer rlist\n");
        return;
    }
    List *next_node=node;
    while(next_node) {
        if(next_node->pointer) renderer_draw(next_node->pointer,camera);
        next_node=next_node->next;
    }
}
List * rlist_optimize(List *node,BSPNode *root){
    if(!root) {
        printf("NULL TREE SCN\n");
        return node;
    }
    
    printf("pau torando 2\n");
    List *next_node=node;
    while(next_node->next) {
        if(next_node->pointer ) { //&& next_node->renderer->is_static
		    printf("pau torando21\n");
            bsp_add(root,next_node->pointer);
            next_node=list_remove(next_node,next_node->pointer);
            printf("pau torando212\n");
            if(!next_node) return NULL;
        } else {
		    printf("pau torando22\n");
            next_node=next_node->next;
        }
    }
    
		printf("pau torando 3\n");
    while(next_node->prev) next_node=next_node->prev;
    return next_node;
}

void bsp_draw(BSPNode *node,Camera *camera) {

    deep_counter++;
    if(!node) return;
        if(graphics_get_debugmode())bsp_debugdraw(camera,node);
    if(node->renderer) {
        renderer_draw(node->renderer,camera);
    } else {
        if(node->left) bsp_draw(node->left,camera);
        if(node->right) bsp_draw(node->right,camera);
    }
    deep_counter--;
}

static void recalculate_boundbox(BSPNode *node) {
    // se ele nao tiver os dois para juntar o boundbox pega apenas daquele que tem
    if(node->renderer) {
         RenderAABB aabb_tmp=node->renderer->render_object->aabb;
        node->size=aabb_tmp.size;
        if(true) {
            GLfloat highvalue=aabb_tmp.size.x;
            if(aabb_tmp.size.y>highvalue) highvalue=aabb_tmp.size.y;
            if(aabb_tmp.size.z>highvalue) highvalue=aabb_tmp.size.z;
            node->size=vec3_create(highvalue,highvalue,highvalue);
        }
        node->position=vec3_add(mat4_get_translation(node->renderer->matrix),aabb_tmp.center_offset);
    }
    if(!(node->left) || !(node->right)) {
        if(node->left) {
            node->size=node->left->size;
            node->position=node->left->position;
        }else if(node->right) {
            node->size=node->right->size;
            node->position=node->right->position;
        } else {
            node->size=vec3_create(0,0,0);
            node->position=vec3_create(0,0,0);
        }
        return;
    }
    // se ele tem entao calcula a soma dos dois
    BSPNode left=*(node->left);
    BSPNode right=*(node->right);

    Vec3 lowestValues=vec3_create(FLT_MAX,FLT_MAX,FLT_MAX);
    Vec3 higgestValues=vec3_create(FLT_MIN,FLT_MIN,FLT_MIN);

    if(left.position.x+(left.size.x/2.0f)<lowestValues.x)
        lowestValues.x=left.position.x+(left.size.x/2.0f);
    if(left.position.x+(left.size.x/2.0f)>higgestValues.x)
        higgestValues.x=left.position.x+(left.size.x/2.0f);

    if(left.position.y+(left.size.y/2.0f)<lowestValues.y)
        lowestValues.y=left.position.y+(left.size.y/2.0f);
    if(left.position.y+(left.size.y/2.0f)>higgestValues.y)
        higgestValues.y=left.position.y+(left.size.y/2.0f);

    if(left.position.z+(left.size.z/2.0f)<lowestValues.z)
        lowestValues.z=left.position.z+(left.size.z/2.0f);
    if(left.position.z+(left.size.z/2.0f)>higgestValues.z)
        higgestValues.z=left.position.z+(left.size.z/2.0f);

    if(right.position.x+(right.size.x/2.0f)<lowestValues.x)
        lowestValues.x=right.position.x+(right.size.x/2.0f);
    if(right.position.x+(right.size.x/2.0f)>higgestValues.x)
        higgestValues.x=right.position.x+(right.size.x/2.0f);

    if(right.position.y+(right.size.y/2.0f)<lowestValues.y)
        lowestValues.y=right.position.y+(right.size.y/2.0f);
    if(right.position.y+(right.size.y/2.0f)>higgestValues.y)
        higgestValues.y=right.position.y+(right.size.y/2.0f);

    if(right.position.z+(right.size.z/2.0f)<lowestValues.z)
        lowestValues.z=right.position.z+(right.size.z/2.0f);
    if(right.position.z+(right.size.z/2.0f)>higgestValues.z)
        higgestValues.z=right.position.z+(right.size.z/2.0f);

    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////

    if(left.position.x-(left.size.x/2.0f)<lowestValues.x)
        lowestValues.x=left.position.x-(left.size.x/2.0f);
    if(left.position.x-(left.size.x/2.0f)>higgestValues.x)
        higgestValues.x=left.position.x-(left.size.x/2.0f);

    if(left.position.y-(left.size.y/2.0f)<lowestValues.y)
        lowestValues.y=left.position.y-(left.size.y/2.0f);
    if(left.position.y-(left.size.y/2.0f)>higgestValues.y)
        higgestValues.y=left.position.y-(left.size.y/2.0f);

    if(left.position.z-(left.size.z/2.0f)<lowestValues.z)
        lowestValues.z=left.position.z-(left.size.z/2.0f);
    if(left.position.z-(left.size.z/2.0f)>higgestValues.z)
        higgestValues.z=left.position.z-(left.size.z/2.0f);

    if(right.position.x-(right.size.x/2.0f)<lowestValues.x)
        lowestValues.x=right.position.x-(right.size.x/2.0f);
    if(right.position.x-(right.size.x/2.0f)>higgestValues.x)
        higgestValues.x=right.position.x-(right.size.x/2.0f);

    if(right.position.y-(right.size.y/2.0f)<lowestValues.y)
        lowestValues.y=right.position.y-(right.size.y/2.0f);
    if(right.position.y-(right.size.y/2.0f)>higgestValues.y)
        higgestValues.y=right.position.y-(right.size.y/2.0f);

    if(right.position.z-(right.size.z/2.0f)<lowestValues.z)
        lowestValues.z=right.position.z-(right.size.z/2.0f);
    if(right.position.z-(right.size.z/2.0f)>higgestValues.z)
        higgestValues.z=right.position.z-(right.size.z/2.0f);

    node->size=vec3_abs(vec3_sub(lowestValues,higgestValues));
    node->position=vec3_lerp(higgestValues,lowestValues,0.5f);

}
static BSPNode *alloc_node(Renderer *renderer) {
    BSPNode *new_node=calloc(1,sizeof(BSPNode));
    calloc(1,sizeof(BSPNode));
    RenderAABB aabb_tmp=renderer->render_object->aabb;
    new_node->size=aabb_tmp.size;
    if(true) {
        GLfloat highvalue=aabb_tmp.size.x;
        if(aabb_tmp.size.y>highvalue) highvalue=aabb_tmp.size.y;
        if(aabb_tmp.size.z>highvalue) highvalue=aabb_tmp.size.z;
        new_node->size=vec3_create(highvalue,highvalue,highvalue);
    }
    new_node->position=vec3_add(mat4_get_translation(renderer->matrix),aabb_tmp.center_offset);
    new_node->renderer=renderer;
    return new_node;
}

void bsp_add(BSPNode *node,Renderer *renderer) {
    if(!node) return;
    // se ele for um no renderer
    if(node->renderer) {
        // copia si mesmo para o no filho direito
        // transforma si mesmo em um no intermediario,
        node->right=alloc_node(node->renderer);
        node->renderer=NULL;
        //alloca o novo no no lado esquerdo
        node->left=alloc_node(renderer);
    } else {
         // se for um no intermediaro completo com left e right
        if(node->left && node->right) {
            // ele procura qual dos dois nos esta mais perto e adiciona ele ali
            Vec3 renderer_position=mat4_get_translation(renderer->matrix);
            if(
                vec3_distance(node->left->position,renderer_position)<
                vec3_distance(node->right->position,renderer_position)
            ) { // left esta mais perto
                bsp_add(node->left,renderer);
            } else { // right esta mais perto
                bsp_add(node->right,renderer);
            }
        } else {
            // se ele for o no raiz ele adiciona no espaco que estiver disponivel
            if(!node->left) {
                node->left=alloc_node(renderer);
            }else {
                node->right=alloc_node(renderer);
            }
        }
    }
    // recalcula o tamanho da caixa do no
    // seja ele um no intermediario ou um no renderer
    recalculate_boundbox(node);
}


/* AQUI OCORRE UM VAZAMENTO BRUTO DE MEMORIA. PRECISA SER IMPLEMENTADAS ESSAS FUNCOES*/
void bsp_remove(BSPNode *node,Renderer *renderer) {

}
BSPNode * bsp_destroy(BSPNode *root) {
    return NULL;
}



RenderScene* scn_alloc() {
    RenderScene *scn=calloc(1,sizeof(RenderScene));
    scn->tree=calloc(1,sizeof(BSPNode));
    return scn;
}
RenderScene* scn_destroy(RenderScene *scn) {
    scn->renderer_list=list_destroy(scn->renderer_list);
    scn->tree=bsp_destroy(scn->tree);
    free(scn);
    return NULL;
}
void scn_draw(RenderScene *scn,Camera *camera) {
    bsp_draw(scn->tree,camera);
    rlist_draw(scn->renderer_list,camera);
}
void scn_add(RenderScene *scn,Renderer *renderer) {
    if(renderer->is_static) {
        bsp_add(scn->tree,renderer);
    } else {
        scn->renderer_list=list_add(scn->renderer_list,renderer);
    }
}
void scn_remove(RenderScene *scn,Renderer *renderer) {
    scn->renderer_list=list_remove(scn->renderer_list,renderer);
    bsp_remove(scn->tree,renderer);
}

void scn_optimize(RenderScene *scn) {
    scn->renderer_list=rlist_optimize(scn->renderer_list,scn->tree); 
}

