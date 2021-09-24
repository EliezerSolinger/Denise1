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
#include "modules/obj_loader.h"
#include <GLFW/glfw3.h>
/*
 * REQUIRE OPENGL 2.1 WITH ARB_VERTEX_ARRAY_OBJECT EXTENSION
 * 
 */


Camera *main_camera=NULL;
Transform tr_x_camera=TRANSFORM_IDENTITY;
Transform tr_y_camera=TRANSFORM_IDENTITY;
Transform tr_base_camera=TRANSFORM_IDENTITY;
UIRect mira=UIRECT_NEW;
UIRect painel=UIRECT_NEW;
UITextBox textbox=UITEXTBOX_NEW;

BSPNode bsp_root=BSPNODE_NEW;

bool engine_init(const char *file) {
	if(!file) {
		printf("ARQUIVO OBJ NAO ESPECIFICADO! %s\n",file);
		return false;
	}
	if(!graphics_init("ModelViewer 0.0.1f")) return false;
	printf("OpenGL started.\n");
	tr_base_camera.position=vec3_create(0,2,15);
	//tr_base_camera.angles.y=M_PI;
	tr_y_camera.angles=vec3_create(0,0,0);	
	tr_x_camera.angles=vec3_create(-0,0,0);	
	
	textbox.texture_font_id=load_texture("fonts/ascii.png",false);
	textbox.color=color4f_create(1,0,0,1);
 	main_camera=camera_alloc();

	Renderer *renderers;
	int rend_count=0;
	load_obj_renderers(file,&renderers,&rend_count);
	
	for(int c=0;c<rend_count;c++) {
    	printf("adicionando renderer %d rendobj %f \n",c,renderers[c].render_object->aabb.size.x);
		bsp_add(&bsp_root,&(renderers[c]));
	}
	painel.rect.size=vec2_create(100,100);
	painel.color=color4f_create(1,1,1,0);
 	return true;
}

void engine_update() {	
	camera_fix_aspect(main_camera);
	camera_draw_viewport(main_camera);
	camera_update_matrix(main_camera);
	
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
	 
	if(input_pressing(GLFW_KEY_LEFT)) tr_y_camera.angles.y-=velocidade;
	if(input_pressing(GLFW_KEY_RIGHT)) tr_y_camera.angles.y+=velocidade;
	if(input_pressing(GLFW_KEY_DOWN)) tr_x_camera.angles.x+=velocidade;
	if(input_pressing(GLFW_KEY_UP)) tr_x_camera.angles.x-=velocidade;
	
	if(input_pressing(GLFW_KEY_U)) textbox.font_size*=1.0f+time_delta();
	if(input_pressing(GLFW_KEY_J)) textbox.font_size*=1.0f-time_delta();

	if(input_pressed(GLFW_KEY_F3)) 
		graphics_set_debugmode(!graphics_get_debugmode());
	Mat4 mov_matrix=MAT4_IDENTITY;
	mat4_translate_vec3(&mov_matrix,movimento);
	mat4_rotate_vec3(&mov_matrix,vec3_create(tr_x_camera.angles.x,tr_y_camera.angles.y,0));
	tr_base_camera.position=vec3_add(tr_base_camera.position,mat4_get_translation(mov_matrix));
	
	//transform_into_view_matrix(tr_base_camera,&(main_camera.view_matrix));
	main_camera->view_matrix=mat4_mult(
		mat4_mult(
			transform_to_view_matrix(tr_x_camera),
			transform_to_view_matrix(tr_y_camera)
		),
		transform_to_view_matrix(tr_base_camera)
	);

	bsp_draw(&bsp_root,main_camera);
	ui_draw_viewport();
	
	painel.rect.position=vec2_create(
		((-50.0f)*window_aspect())+(painel.rect.size.x/2.0f)
		,(50.0f)-(painel.rect.size.y/2.0f)
	);
	textbox.rect=painel.rect;
	ui_rect_draw(&painel);
	char window_title[1000];
	textbox.text=window_title;
	Vec3 cam_pos=tr_base_camera.position;
	snprintf(window_title,1000,"FPS %d cam(%d,%d,%d) \n %d renderers %d triangles \n",time_fps(),(int)cam_pos.x,(int)cam_pos.y,(int)cam_pos.z,(int)(main_camera->stats_drawcalls),(int)(main_camera->stats_triangles_drawed));
	
	textbox.text_length=strlen(window_title);
	ui_text_draw(&textbox);
}
void engine_destroy() {
	
	main_camera=camera_destroy(main_camera);
	graphics_destroy();
}

int main(int argc, char **argv) {
	if(argc==1) {
		printf("ERROR FILE NOT SPECIFIED!\n");
		return -1;
	}
	if(!engine_init(argv[1])) return -1;

	while(graphics_update()) {
		engine_update();
	}

	engine_destroy(); 
	return 0;
}
