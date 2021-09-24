#include "core/mathematics.h"
#include "graphics/system/window.h"
#include "camera3d.h"
#include "stdlib.h"

Camera3D *camera3d_alloc() {
	Camera3D *this=malloc(sizeof(Camera3D));
	this->clear_color=color4f_create(0.6f,0.75f,1.0f,1.0f);
	this->is_perspective=true;
	this->fovy=70;
	this->aspect=1;
	this->left=50;
	this->right=-50;
	this->top=50;
	this->bottom=-50;
	this->zNear=0.1;
	this->zFar=1000;
	this->projection_matrix=mat4_identity;
	this->view_matrix=mat4_identity;
    this->render_layer=0;
    this->viewport=rect_empty;
	this->stats_drawcalls=0;
	this->stats_triangles_drawed=0;
    camera_update_matrix(this);
	return this;
}
Camera3D *camera3d_destroy(Camera3D *camera3d) {
	free(camera3d);
	return NULL;
}
void camera3d_update_matrix(Camera3D *camera) {
	if(camera->is_perspective) {
		camera->projection_matrix=
			mat4_perspective(camera->fovy,camera->aspect,camera->zNear,camera->zFar);
	} else {
		camera->projection_matrix=
			mat4_ortho(camera->left,camera->right,camera->bottom,camera->top,camera->zNear,camera->zFar);
	}
}
	

void camera3d_perspective(Camera3D *camera,GLfloat fovy,GLfloat aspect,GLfloat zNear,GLfloat zFar) {
	camera->fovy=fovy;
	camera->aspect=aspect;
	camera->zNear=zNear;
	camera->zFar=zFar;
	camera_update_matrix(camera);
}

void camera3d_ortho(
			Camera3D *camera,
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
	camera3d_update_matrix(camera);
}

Mat4 camera3d_generate_shader_matrix(Camera3D *camera3d,Mat4 model) {
	Transform2D camt=transform2d_identity;
	camt.position=camera3d->position;
	camt.rotation=camera3d->rotation;
	//mat4_scale_vec3(&(camera3d->view_matrix),vec3_create(-1,-1,-1));
	return mat4_mult(
		camera3d->projection_matrix,
		mat4_mult(transform2d_to_view_matrix(camt),model)
	);
}

void camera3d_draw_viewport(Camera3D *camera3d) {
	camera3d->stats_drawcalls=0;
	camera3d->stats_triangles_drawed=0;
	Rect viewport=camera3d->viewport;
	glViewport(
		viewport.position.x*window_width(),
		viewport.position.y*window_height(),
		viewport.size.x*window_width(),
		viewport.size.y*window_height()
	);
	Color4f sky_color=camera3d->clear_color;
	glClearColor(sky_color.r,sky_color.g,sky_color.b,sky_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
void camera3d_fix_aspect(Camera3D *camera3d) {
	camera3d->aspect=window_aspect();
	camera3d_update_matrix(camera3d);
	//printf("Ajustando aspecto da camera xy para %f.",camera3d->aspect);
}
