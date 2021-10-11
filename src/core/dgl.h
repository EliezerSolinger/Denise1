

#ifndef NK_OPENGL_SHADERS_H
#define NK_OPENGL_SHADERS_H
#include <glad/glad.h>

#define SHADER_INLINE(shader)  "#version 120\n" #shader

GLuint shader_compile_vertex(const char *shadersource);
GLuint shader_compile_fragment(const char *shadersource);
GLuint shader_link_program(GLuint vertex_shader,GLuint fragment_shader);
GLuint shader_create_program(const char *vertex_shader,const char * fragment_shader);

#endif


#ifndef GROTA_GRAPHICS_H
#define GROTA_GRAPHICS_H
#include <glad/glad.h>
#include <stdbool.h>

GLuint graphics_null_textureid();
void graphics_bind_null_texture();
void graphics_bind_blank_texture();
bool graphics_init();

GLuint* load_texture(const char *filename,bool linear);
void texture_destroy(GLuint *texture_id);
#endif