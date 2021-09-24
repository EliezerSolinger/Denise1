#ifndef GROTA_CAMERA2D_H
#define GROTA_CAMERA2D_H

#include "core/mathematics.h"
#include <stdbool.h>
// the object camera3d can be perspective or orthogonal,
// if is_perspective are true then the camera3d is perspective
// otherwise orthogonal
typedef struct _Camera3D{
	Color4f clear_color;
	bool is_perspective;
	// only for perspective configuration
	GLfloat fovy, aspect;
	// only for orthogonal configuration
	GLfloat left,right,top,bottom;
	// for any type
	GLfloat zNear,zFar;
	Mat4 projection_matrix;
	Mat4 view_matrix;
	GLint render_layer;
	Rect viewport;
	// statistics
	GLuint stats_drawcalls;
	GLuint stats_triangles_drawed;
} Camera3D;

Camera3D *camera3d_alloc();
Camera3D *camera3d_destroy(Camera3D *camera3d);

void camera3d_perspective(Camera *camera,GLfloat fovy,GLfloat aspect,GLfloat zNear,GLfloat zFar);

void camera3d_ortho(
			Camera *camera,
			GLfloat left,
			GLfloat right,
			GLfloat bottom,
			GLfloat top,
			GLfloat zNear,
			GLfloat zFar);
			
void camera3d_update_matrix(Camera3D *camera3d);

Mat4 camera3d_generate_shader_matrix(Camera3D *camera3d,Mat4 model);
void camera3d_fix_aspect(Camera3D *camera3d);
void camera3d_draw_viewport(Camera3D *cam);

#endif
