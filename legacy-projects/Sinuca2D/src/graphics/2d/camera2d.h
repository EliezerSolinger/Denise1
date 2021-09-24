#ifndef GROTA_CAMERA2D_H
#define GROTA_CAMERA2D_H

#include "core/mathematics.h"
#include <stdbool.h>
// the object camera2d can be perspective or orthogonal,
// if is_perspective are true then the camera2d is perspective
// otherwise orthogonal
typedef struct _Camera2D{
	Vec2 position;
	float rotation;
	Color4f clear_color;
	GLfloat zoom, aspect;
	Mat4 projection_matrix;
	GLint render_layer;
	Rect viewport;
	// statistics
	GLuint stats_drawcalls;
	GLuint stats_triangles_drawed;
} Camera2D;

Camera2D *camera2d_alloc();
Camera2D *camera2d_destroy(Camera2D *camera2d);

void camera2d_update_matrix(Camera2D *camera2d);

Mat4 camera2d_generate_shader_matrix(Camera2D *camera2d,Mat4 model);
void camera2d_fix_aspect(Camera2D *camera2d);
void camera2d_draw_viewport(Camera2D *cam);

#endif
