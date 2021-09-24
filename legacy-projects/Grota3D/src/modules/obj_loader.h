#ifndef GROTA_OBJ_LOADER_H
#define GROTA_OBJ_LOADER_H
#include "graphics/3d/renderer.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBJ_MAX_VERTICES 4096
#define OBJ_MAX_TRIANGLES 4096
#define OBJ_MAX_MESHES 64
#define MTL_MAX_MATERIALS 64
/*
Ka color_ambient
Kd color_diffuse
Ks color_specular
Ke color_emmissive
d alpha_value 	

Ns shineness
Ni index of refraction

m* texture
map_kd diffuse texture
map_ka ambient texture
map_ks specular texture
map_ke emissive texture
map_bump bump texture
map_ns glossiness texture

n* new material name
i* ilumination model

*/
typedef struct MTLMaterial {
    char *name;
    unsigned int name_length;
    Color4f ambient;
    Color4f diffuse;
    Color4f specular;
    GLfloat shineness;
    //Vec3 emissive;
    int illum;
    char *diffuse_texture_path;
    unsigned int diffuse_texture_path_length;
} MTLMaterial;

typedef struct MTLFile {
   /* char *name;
    unsigned int name_length;*/
    MTLMaterial * materials;
    unsigned int materials_count;
} MTLFileData;

typedef struct {
    GLuint vertexIndices[3];
    GLuint uvIndices[3];
    GLuint normalIndices[3];
    bool with_error;
} OBJTriangle;

typedef struct OBJMesh {
    char *name;
    GLuint name_length;
    Vec3 * vertices;
    GLuint vertices_count;
    Vec3 * normals;
    GLuint normals_count;
    Vec2 * uvs;
    GLuint uvs_count;
    OBJTriangle * triangles;
    GLuint triangles_count;
    GLuint group_id;
    char *material_name;
    GLuint material_name_length;
} OBJMesh;


typedef struct OBJFileData {
    //char *name;
    //GLuint name_length;
    char *mtl_path;
    GLuint mtl_path_length;
    OBJMesh *objects;
    GLuint objects_count;
} OBJFileData;


MTLFileData *mtl_loader(const char *filename);
bool obj_loader(const char * filename,OBJFileData *objdata);
bool load_obj_first_mesh(const char *filename,MeshData *out_mesh);
bool load_obj_renderers(const char *obj_filename,Renderer **renderers,int *renderers_count);

#endif