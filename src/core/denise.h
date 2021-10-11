


#ifndef GROTA_CAMERA3D_H
#define GROTA_CAMERA3D_H

#include "dmath.h"
#include <stdbool.h>
#include <stdint.h>
#include <glad/glad.h>
// the object camera can be perspective or orthogonal,
// if is_perspective are true then the camera is perspective
// otherwise orthogonal
typedef struct _Camera{
	Color4f clear_color;
	bool is_perspective;
	// only for perspective configuration
	float fovy, aspect;
	// only for orthogonal configuration
	float left,right,top,bottom;
	// for any type
	float zNear,zFar;
	Mat4 projection_matrix;
	Mat4 view_matrix;
	int render_layer;
	Rect viewport;
	// statistics
	unsigned int stats_drawcalls;
	unsigned int stats_triangles_drawed;
} Camera;

Camera *camera_alloc();
Camera *camera_destroy(Camera *camera);

void camera_perspective(Camera *camera,float fovy,float aspect,float zNear,float zFar);

void camera_ortho(
			Camera *camera,
			float left,
			float right,
			float bottom,
			float top,
			float zNear,
			float zFar);
			
void camera_update_matrix(Camera *camera);

Mat4 camera_generate_shader_matrix(Camera *camera,Mat4 model);
void camera_fix_aspect(Camera *camera);
void camera_draw_viewport(Camera *cam);

#endif



#ifndef GROTA_RENDERER_H
#define GROTA_RENDERER_H

#include "dmath.h"


typedef struct _Material{
    Color4f diffuse;
    Color4f ambient;
    Color4f specular;
    float shineness;
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
    uint32_t vertices_count;
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