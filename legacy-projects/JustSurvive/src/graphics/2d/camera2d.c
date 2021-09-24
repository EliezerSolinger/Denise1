#include "core/mathematics.h"
#include "graphics/system/window.h"
#include "camera2d.h"
#include "stdlib.h"
Camera2D *camera2d_alloc() {
	Camera2D *camera2d=malloc(sizeof(Camera2D));
	camera2d->clear_color=color4f_create(0.6f,0.75f,1.0f,1.0f);
	camera2d->zoom=10;
	camera2d->aspect=1;
	camera2d->projection_matrix=mat4_identity;
	//camera2d->view_matrix=mat4_identity;
	camera2d->position=vec2_create(0,0);
	camera2d->rotation=0;
    camera2d->render_layer=0;
    camera2d->viewport=rect_empty;
	camera2d->stats_drawcalls=0;
	camera2d->stats_triangles_drawed=0;
    camera2d_update_matrix(camera2d);
	return camera2d;
}

Camera2D *camera2d_destroy(Camera2D *camera2d) {
	free(camera2d);
	return NULL;
}
void camera2d_update_matrix(Camera2D *camera2d) {
	camera2d->projection_matrix=mat4_create(1.0f);
	mat4_scale_vec2(&(camera2d->projection_matrix),vec2_create(1.0f/(camera2d->zoom*camera2d->aspect),1.0f/(camera2d->zoom)));
}
	

Mat4 camera2d_generate_shader_matrix(Camera2D *camera2d,Mat4 model) {
	Transform2D camt=transform2d_identity;
	camt.position=camera2d->position;
	camt.rotation=camera2d->rotation;
	//mat4_scale_vec3(&(camera2d->view_matrix),vec3_create(-1,-1,-1));
	return mat4_mult(
		camera2d->projection_matrix,
		mat4_mult(transform2d_to_view_matrix(camt),model)
	);
}

void camera2d_draw_viewport(Camera2D *camera2d) {
	camera2d->stats_drawcalls=0;
	camera2d->stats_triangles_drawed=0;
	Rect viewport=camera2d->viewport;
	glViewport(
		viewport.position.x*window_width(),
		viewport.position.y*window_height(),
		viewport.size.x*window_width(),
		viewport.size.y*window_height()
	);
	Color4f sky_color=camera2d->clear_color;
	glClearColor(sky_color.r,sky_color.g,sky_color.b,sky_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}
void camera2d_fix_aspect(Camera2D *camera2d) {
	camera2d->aspect=window_aspect();
	camera2d_update_matrix(camera2d);
	//printf("Ajustando aspecto da camera xy para %f.",camera2d->aspect);
}
