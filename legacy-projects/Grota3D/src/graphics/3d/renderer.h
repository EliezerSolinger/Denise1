#ifndef GROTA_RENDERER_H
#define GROTA_RENDERER_H

#include "core/mathematics.h"
#include "core/colors.h"
#include "graphics/3d/camera.h"


typedef struct _Material{
    Color4f diffuse;
    Color4f ambient;
    Color4f specular;
    GLfloat shineness;
    GLuint texture_id;
    Vec2 texture_scale;
}Material;

#define MATERIAL_DEFAULT {COLOR4F_WHITE,COLOR4F_WHITE,{0,0,0,0},0,0,VEC2_FILL_ONE}

typedef struct _VertexData{
    Vec3 vertex;
    Vec3 normal;
    Vec2 texture_coordinates;
} VertexData;

typedef struct _MeshData{
    VertexData *vertices;
    GLuint vertices_count;
} MeshData;

typedef struct _RenderAABB{
    Vec3 size;
    Vec3 center_offset;
} RenderAABB;
#define RENDERAABB_NEW {VEC3_FILL_ONE,VEC3_ZERO}



// i need EBO and VBO to clear
typedef struct _RenderObject{
    GLuint VAO;
    GLuint VBO;
    GLuint vertices_count;
    RenderAABB aabb;
} RenderObject;
#define RENDEROBJECT_NEW {0,0,0,RENDERAABB_NEW}

typedef struct _Renderer{
    Material material;
	Mat4 matrix;
    RenderObject *render_object;
    bool is_static;
} Renderer;
 
#define RENDERER_NEW {MATERIAL_DEFAULT,MAT4_IDENTITY,0,false}


void debugbox_draw(Camera *camera,Mat4 matrix,Color4f color);
void debugsphere_draw(Camera *camera,Mat4 matrix,Color4f color);

void mesh_freecontent(MeshData *mesh);

RenderObject mesh_load(MeshData *mesh);

void renderobject_unload(RenderObject *obj);

void renderer_draw(Renderer *renderer,Camera *camera);

void renderer_init_quad(Renderer *quad);

void renderer_init_box(Renderer *box);



#endif