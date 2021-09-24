#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "core/mathematics.h"
#include "core/time.h"
#include "graphics/system/window.h"
#include "graphics/graphics.h"
#include "graphics/3d/camera.h"
#include "graphics/3d/renderer.h"
#include "graphics/ui/ui.h"
#include "graphics/3d/bsp.h"
#include "physics/aabb.h"
#include "modules/obj_loader.h"
#include <GLFW/glfw3.h>

static void draw_sphere(SphereCollider *bola,Camera *camera,bool colidindo) {
	Mat4 mat=MAT4_IDENTITY;
	mat4_translate_vec3(&mat,bola->position);
	mat4_scale_vec3(&mat,vec3_create(bola->radius,bola->radius,bola->radius));
	static Color4f red=COLOR4F_RED;
	static Color4f green=COLOR4F_GREEN;
	if(colidindo) {
		debugsphere_draw(camera,mat,red);
	} else {
		debugsphere_draw(camera,mat,green);
	}
}
static void draw_aabb(AABBCollider *aabb,Camera *camera,bool colidindo) {
	Mat4 mat=MAT4_IDENTITY;
	mat4_translate_vec3(&mat,aabb->position);
	mat4_scale_vec3(&mat,aabb->size);
	static Color4f red=COLOR4F_RED;
	static Color4f green=COLOR4F_GREEN;
	if(colidindo) {
		debugbox_draw(camera,mat,red);
	} else {
		debugbox_draw(camera,mat,green);
	}
}
int main(int argc, char **argv) {
    static Camera *camera;
	camera=camera_alloc();
    camera_perspective(camera,70,1,0.05,1000);
	Transform tr_camera=TRANSFORM_IDENTITY;
	
	tr_camera.position.y=3;
	tr_camera.angles.x=M_PI/10;
	transform_into_view_matrix(tr_camera,&(camera->view_matrix));

    printf("initializing system...\n");
    if(!graphics_init(NULL)) return false;
	printf("OpenGL started.\n");


	AABBCollider *cola=collider_aabb_alloc();
	cola->position.y=3;
	cola->position.z=-10;

	AABBCollider *colb=collider_aabb_alloc();
	colb->position.z=-10;
	colb->position.y=-2;

	SphereCollider *bola=collider_sphere_alloc();
	bola->position=vec3_create(0,4,-10);
	
	List *colliders=NULL;
	colliders=list_add(colliders,cola);
	colliders=list_add(colliders,colb);
	colliders=list_add(colliders,colb);

	while(graphics_update()) {		
		camera_draw_viewport(camera);
		camera_fix_aspect(camera);
		if(input_pressed(GLFW_KEY_F)) cola->position.y=3;
		bool colide=detect_colision(cola,colb);	
		if(!colide) 
			cola->position.y-=time_delta()/5.0f; // 25cm/s
		bool colidebola=detect_colision(colb,bola) || detect_colision(cola,bola);
		if(!colidebola) 
			bola->position.y-=time_delta()/8.0f; // 20cm/s

		draw_sphere(bola,camera,colidebola);
		draw_aabb(cola,camera,colide);
		draw_aabb(colb,camera,colide);
	}
	collider_sphere_destroy(bola);
	collider_aabb_destroy(cola);
	collider_aabb_destroy(colb);
	graphics_destroy();
	return 0;
}



