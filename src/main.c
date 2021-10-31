#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>
#include "core/dmath.h"
#include "core/dsystem.h"
#include "core/dgl.h"
#include "core/denise.h"

Camera *main_camera=NULL;

Transform3D tr_x_camera=TRANSFORM3D_IDENTITY;
Transform3D tr_y_camera=TRANSFORM3D_IDENTITY;
Transform3D tr_base_camera=TRANSFORM3D_IDENTITY;

int main() {
	if(!window_init("MY FIRST GAME")) return false;
    graphics_init();
	tr_base_camera.position=vec3_create(0,2,15);
	//tr_base_camera.angles.y=M_PI;
	tr_y_camera.rotation=vec3_create(0,0,0);	
	tr_x_camera.rotation=vec3_create(-0,0,0);	
	
    main_camera=camera_alloc();

    camera_perspective(main_camera,70,1,0.05,1000);
    while(window_update()) {
        if(input_pressed(GLFW_KEY_ESCAPE)) window_close();

    	// update projection
	float velocidade=time_delta()*1.5; //meio metro por segundo
	
	if(input_pressing(GLFW_KEY_LEFT_SHIFT)) {
		velocidade*=3;
	}
	Mat4* cmat=&(main_camera->view_matrix);
 
	Vec3 movimento=VEC3_ZERO;

    if(input_pressing(GLFW_KEY_W)) movimento.z-=velocidade;
    if(input_pressing(GLFW_KEY_S)) movimento.z+=velocidade;

    if(input_pressing(GLFW_KEY_D)) movimento.x+=velocidade;
    if(input_pressing(GLFW_KEY_A)) movimento.x-=velocidade;

    if(input_pressing(GLFW_KEY_E))	movimento.y-=velocidade;
    if(input_pressing(GLFW_KEY_Q)) movimento.y+=velocidade;
    
    if(input_pressing(GLFW_KEY_LEFT)) tr_y_camera.rotation.y-=velocidade;
    if(input_pressing(GLFW_KEY_RIGHT)) tr_y_camera.rotation.y+=velocidade;
    if(input_pressing(GLFW_KEY_DOWN)) tr_x_camera.rotation.x+=velocidade;
    if(input_pressing(GLFW_KEY_UP)) tr_x_camera.rotation.x-=velocidade;
        
    Mat4 mov_matrix=MAT4_IDENTITY;
	mat4_translate_vec3(&mov_matrix,movimento);
	mat4_rotate_vec3(&mov_matrix,vec3_create(tr_x_camera.rotation.x,tr_y_camera.rotation.y,0));
	tr_base_camera.position=vec3_add(tr_base_camera.position, mat4_get_translation(mov_matrix));
	
	   /* main_camera->view_matrix=transform3d_to_view_matrix(tr_base_camera);*/
        main_camera->view_matrix=mat4_mult(
            mat4_mult(
                transform3d_to_view_matrix(tr_x_camera),
                transform3d_to_view_matrix(tr_y_camera)
            ),
            transform3d_to_view_matrix(tr_base_camera)
        );   

        camera_fix_aspect(main_camera);
        camera_draw_viewport(main_camera);
        camera_update_matrix(main_camera);

        debugbox_draw(main_camera,mat4_create(1.0f),COLOR4F_WHITE);    
    }
    window_destroy();
    return 0;
}

