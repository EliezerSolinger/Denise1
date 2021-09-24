#ifndef GROTA_GRAPHICS_H
#define GROTA_GRAPHICS_H
#include <glad/glad.h>
#include <stdbool.h>

void graphics_set_debugmode(bool value);
bool graphics_get_debugmode();
GLuint graphics_null_textureid();
void graphics_bind_null_texture();
void graphics_bind_blank_texture();
bool graphics_init(const char *window_title,bool antialiasing);
bool graphics_update();
void graphics_destroy();

GLuint* load_texture(const char *filename,bool linear);
void texture_destroy(GLuint *texture_id);
#endif