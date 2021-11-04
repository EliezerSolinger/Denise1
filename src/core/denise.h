


#ifndef GROTA_CAMERA3D_H
#define GROTA_CAMERA3D_H

#include "dmath.h"
#include "structs.h"
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
			
void camera_update_projection_matrix(Camera *camera);

Mat4 camera_generate_shader_matrix(Camera *camera,Mat4 model);
void camera_fix_aspect(Camera *camera);
void camera_draw_viewport(Camera *cam);

#endif



#ifndef GROTA_RENDERER_H
#define GROTA_RENDERER_H

#include "dmath.h"

typedef GLuint TextureRef;

typedef enum _LightType{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
	AMBIENT_LIGHT
} LightType;

typedef struct _Light {
	LightType type;
	Color4f color;
	Vec3 position;
	Vec3 limits;
	Vec3 direction;
	float intensity;
} Light;

typedef struct _Scene {
	List *lights;
} Scene;


typedef struct _Material{
    Color4f diffuse;
    Color4f specular;
    float shineness;
	TextureRef albedo_texture;
	TextureRef normal_texture;
    Vec2 texture_scale;
} Material;

static const Material MATERIAL_DEFAULT = {COLOR4F_WHITE,COLOR4F_WHITE,0,0,0,VEC2_FILL_ONE};

typedef struct _VertexData{
    Vec3 vertex;
    Vec3 normal;
    Vec2 texture_coordinates;
} VertexData;

typedef struct _MeshData{
    VertexData *vertices;
    uint32_t vertices_count;
} MeshData;




typedef struct _RenderObject{
    GLuint VBO;
    GLuint vertices_count;
} RenderObject;
static const RenderObject RENDEROBJECT_NEW = {0U,0U};

typedef struct _Renderer{
    Material material;
	Mat4 matrix;
    RenderObject *render_object;
} Renderer;
 
static const Renderer RENDERER_NEW = {MATERIAL_DEFAULT,MAT4_IDENTITY,0};


Renderer *renderer_debugbox();
Renderer *renderer_debugsphere();
void mesh_freecontent(MeshData *mesh);

RenderObject mesh_load(MeshData *mesh);

void renderobject_unload(RenderObject *obj);

void renderer_draw(Renderer *renderer,Camera *camera);




#endif