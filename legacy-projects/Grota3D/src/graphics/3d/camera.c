#include "core/mathematics.h"
#include "core/colors.h"
#include "graphics/system/window.h"
#include "camera.h"
#include "stdlib.h"
 Camera *camera_alloc() {
	Camera *camera=malloc(sizeof(Camera));
	camera->clear_color=color4f_create(0.6f,0.75f,1.0f,1.0f);
	camera->is_perspective=true;
	camera->fovy=70;
	camera->aspect=1;
	camera->left=50;
	camera->right=-50;
	camera->top=50;
	camera->bottom=-50;
	camera->zNear=0.1;
	camera->zFar=1000;
	camera->projection_matrix=mat4_identity;
	camera->view_matrix=mat4_identity;
    camera->render_layer=0;
    camera->viewport=rect_empty;
	camera->stats_drawcalls=0;
	camera->stats_triangles_drawed=0;
    camera_update_matrix(camera);
	return camera;
}

Camera *camera_destroy(Camera *camera) {
	free(camera);
	return NULL;
}
void camera_update_matrix(Camera *camera) {
	if(camera->is_perspective) {
		camera->projection_matrix=
			mat4_perspective(camera->fovy,camera->aspect,camera->zNear,camera->zFar);
	} else {
		camera->projection_matrix=
			mat4_ortho(camera->left,camera->right,camera->bottom,camera->top,camera->zNear,camera->zFar);
	}
}
	
void camera_perspective(Camera *camera,GLfloat fovy,GLfloat aspect,GLfloat zNear,GLfloat zFar) {
	camera->fovy=fovy;
	camera->aspect=aspect;
	camera->zNear=zNear;
	camera->zFar=zFar;
	camera_update_matrix(camera);
}

void camera_ortho(
			Camera *camera,
			GLfloat left,
			GLfloat right,
			GLfloat bottom,
			GLfloat top,
			GLfloat zNear,
			GLfloat zFar) {				
	camera->left=left;
	camera->right=right;
	camera->bottom=bottom;
	camera->top=top;
	camera->zNear=zNear;
	camera->zNear=zNear;
	camera_update_matrix(camera);
}


Mat4 camera_generate_shader_matrix(Camera *camera,Mat4 model) {
	Mat4 inverse_view_matrix=camera->view_matrix;
	//mat4_scale_vec3(&(camera->view_matrix),vec3_create(-1,-1,-1));
	return mat4_mult(
		camera->projection_matrix,
		mat4_mult(camera->view_matrix,model)
	);
}

void camera_draw_viewport(Camera *camera) {
	camera->stats_drawcalls=0;
	camera->stats_triangles_drawed=0;
	Rect viewport=camera->viewport;
	glViewport(
		viewport.position.x*window_width(),
		viewport.position.y*window_height(),
		viewport.size.x*window_width(),
		viewport.size.y*window_height()
	);
	Color4f sky_color=camera->clear_color;
	glClearColor(sky_color.r,sky_color.g,sky_color.b,sky_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
void camera_fix_aspect(Camera *camera) {
	camera->aspect=window_aspect();
}
void camera_log(Camera *cam) {
/*	printf(" 
	camera->fovy=70;
	camera->aspect=1;
	camera->left=50;
	camera->right=-50;
	camera->top=50;
	camera->bottom=-50;
	camera->zNear=0.1;
	camera->zFar=1000;",
	" 
	camera->fovy,
	camera->aspect=,
	camera->left=50,
	camera->right=-50,
	camera->top=50,
	camera->bottom=-50,
	camera->zNear=0.1,
	camera->zFar=1000,
	)*/
}

