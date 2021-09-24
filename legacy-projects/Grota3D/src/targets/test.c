#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "core/mathematics.h"
#include "core/time.h"
#include "core/list.h"
#include "graphics/system/window.h"
#include "graphics/graphics.h"
#include "graphics/3d/camera.h"
#include "graphics/3d/renderer.h"
#include "graphics/ui/ui.h"
#include "graphics/3d/bsp.h"
#include "modules/obj_loader.h"
#include "modules/heightmap.h"
#include <GLFW/glfw3.h>
/*
 * REQUIRE OPENGL 2.1 WITH ARB_VERTEX_ARRAY_OBJECT EXTENSION
 * 
 */

Camera *main_camera=NULL;
Renderer box=RENDERER_NEW;
Renderer suzanne=RENDERER_NEW;
Renderer suzanne2=RENDERER_NEW;
Renderer rmap=RENDERER_NEW;

Vec3 aviao_velocidade={0,0,50};
Transform tr_x_aviao=TRANSFORM_IDENTITY;
Transform tr_y_aviao=TRANSFORM_IDENTITY;
Transform tr_z_camera=TRANSFORM_IDENTITY;
Transform tr_x_camera=TRANSFORM_IDENTITY;
Transform tr_y_camera=TRANSFORM_IDENTITY;
Transform tr_base_camera=TRANSFORM_IDENTITY;
UIRect mira=UIRECT_NEW;
UIRect painel=UIRECT_NEW;
UITextBox textbox=UITEXTBOX_NEW;
RenderScene *scn=NULL;

Renderer *renderers_aviao=NULL;
int renderer_aviao_count=0;
Mat4 matriz_aviao=MAT4_IDENTITY;
void aviao_update(){
	
	Mat4 mov_matrix=MAT4_IDENTITY;
	mat4_translate_vec3(&mov_matrix,aviao_velocidade);
	mat4_rotate_vec3(&mov_matrix,vec3_create(tr_x_aviao.angles.x,tr_y_aviao.angles.y,0));

	Vec3 movimento_relativo=mat4_get_translation(mov_matrix);

	tr_y_aviao.position=vec3_add(tr_y_aviao.position,vec3_scalar_mult(movimento_relativo,-time_delta()));

	tr_x_aviao.angles.z=lerp(tr_x_aviao.angles.z,tr_y_camera.angles.y-tr_y_aviao.angles.y,time_delta());
	tr_base_camera.position=vec3_lerp(tr_base_camera.position,tr_y_aviao.position,time_delta()*aviao_velocidade.z/2.0f);
	
	tr_y_aviao.angles.y=lerp(tr_y_aviao.angles.y,tr_y_camera.angles.y,time_delta()/2.0f);
	tr_x_aviao.angles.x=lerp(tr_x_aviao.angles.x,tr_x_camera.angles.x,time_delta());
	
	matriz_aviao=mat4_mult(
		transform_to_matrix(tr_y_aviao),
		transform_to_matrix(tr_x_aviao)
	);
	for(int c=0;c<renderer_aviao_count;c++) {
		renderers_aviao[c].matrix=matriz_aviao;
		renderer_draw(&(renderers_aviao[c]),main_camera);
	}
}

bool engine_init() {
	scn=scn_alloc();
	//tr_y_camera.position.z=20;
	//tr_y_camera.position.y=1;
	//tr_x_camera.position.z=20;
	tr_y_aviao.position.y=20;
	tr_x_aviao.position.y=-3;
	tr_z_camera.position.z=20;
	if(!graphics_init(NULL)) return false;
	printf("OpenGL started.\n");

	/*tr_base_camera.position=vec3_create(1.5,2,-5);*/
	//tr_x_camera.position.z=-20;
	//tr_y_camera.angles.y=M_PI;
	/*tr_y_camera.angles=vec3_create(0,0.35,0);	
	tr_x_camera.angles=vec3_create(-0.16,0,0);	*/
	MeshData suzanne_mesh;
	if(!load_obj_first_mesh("obj/suzanne.obj",&suzanne_mesh)) return false;
	
	HeightMap *htmap=htmap_load("terrain/hmap_30.jpg");
	Rect n=rect_empty;
	MeshData *terrain=htmap_generate_mesh(htmap,rect_empty,256);
	rmap.is_static=true;
	rmap.render_object=calloc(1,sizeof(RenderObject));
	*(rmap.render_object)=mesh_load(terrain);
	rmap.material.texture_id=load_texture("terrain/lmap_30.jpg",true);
	mesh_freecontent(terrain);
	free(terrain);
	
	mat4_translate_vec3(&(rmap.matrix),vec3_create(-2048,0,-2048));
	mat4_scale_vec3(&(rmap.matrix),vec3_create(4096,90,4096));
	//scn_add(scn,&rmap);
	
	textbox.texture_font_id=load_texture("fonts/ascii.png",false);
	textbox.color=color4f_create(1,0,0,1);
	renderer_init_box(&box);
	printf("pitstop 6\n");
	static RenderObject suzanne_obj=RENDEROBJECT_NEW;
	suzanne_obj=mesh_load(&suzanne_mesh);
	suzanne.render_object=&suzanne_obj;

	printf("pitstop 7\n");
	mat4_translate_vec3(&(suzanne.matrix),vec3_create(0,2,0));
	suzanne.material.diffuse=color4f_create(0.4f,0.3f,0.14f,1.0f);
	//mesh_freecontent(&suzanne_mesh);
 	//camera_init(main_camera);
	main_camera=camera_alloc();	
	box.material.diffuse=color4f_create(1,1,1,1);
	suzanne.material.texture_id=load_texture("textures/m2.png",false);
	if(!suzanne.material.texture_id) {
		printf("Failed to load texture\n");
	}
	
	box.material.texture_id=load_texture("textures/targ.png",false);
	if(!box.material.texture_id) {
		printf("Failed to load texture\n");
	};
	
	printf("pitstop 8\n");
	mira.black_as_alpha=true;
	mira.rect.size=vec2_create(5,5);
	mira.texture_id=box.material.texture_id;
	mira.corner_x=Xcenter;
	mira.corner_y=Ytop;
	painel.rect.size=vec2_create(50,10);
	painel.texture_id=suzanne.material.texture_id;

	suzanne2=suzanne;
	mat4_translate_vec3(&suzanne2.matrix,vec3_create(3,0,0));
	suzanne2.material.diffuse=color4f_create(0,0,1,1);
	//bsp_add(&bsp_root,&box);
	//bsp_add(&bsp_root,&suzanne);
	//bsp_add(&bsp_root,&suzanne2);
	Renderer *renderers;
	int rend_count=0;
	load_obj_renderers("obj/arvore.obj",&renderers,&rend_count);
	
	
	load_obj_renderers("obj/alb1/alb1.obj",&renderers_aviao,&renderer_aviao_count);


	for(int c=0;c<rend_count;c++) {
    	printf("adicionando renderer %d rendobj %f \n",c,renderers[c].render_object->aabb.size.x);
		//list=list_add(list,&(renderers[c]));
		//rlist_fst=rlist_add(rlist_fst,&(renderers[c]));
		scn_add(scn,&(renderers[c]));
	}
 	return true;
}

static void alloc_new_box() {
	Renderer *r=calloc(1,sizeof(Renderer));
	*r=suzanne2;
	mat4_translate_vec3(&r->matrix,vec3_create((randf()-0.5f)*60,(randf()-0.5f)*60,(randf()-0.5f)*60));
	scn_add(scn,r);
	//rlist_fst=rlist_add(rlist_fst,r);
}
void engine_update() {	
	camera_fix_aspect(main_camera );
	camera_draw_viewport(main_camera);
	camera_update_matrix(main_camera);
	// update projection
	float velocidade=time_delta()*1.5; //meio metro por segundo
	
	if(input_pressing(GLFW_KEY_LEFT_SHIFT)) {
		velocidade*=3;
	}
	Mat4* cmat=&(main_camera->view_matrix);
 
	Vec3 movimento=VEC3_ZERO;

	if(input_pressing(GLFW_KEY_W)) movimento.z+=velocidade;
	if(input_pressing(GLFW_KEY_S)) movimento.z-=velocidade;

	if(input_pressing(GLFW_KEY_D)) movimento.x+=velocidade;
	if(input_pressing(GLFW_KEY_A)) movimento.x-=velocidade;

	if(input_pressing(GLFW_KEY_E))	movimento.y+=velocidade;
	if(input_pressing(GLFW_KEY_Q)) movimento.y-=velocidade;
	 
	if(input_pressing(GLFW_KEY_LEFT)) tr_y_camera.angles.y-=velocidade*0.4;
	if(input_pressing(GLFW_KEY_RIGHT)) tr_y_camera.angles.y+=velocidade*0.4;
	if(input_pressing(GLFW_KEY_DOWN)) tr_x_camera.angles.x-=velocidade;
	if(input_pressing(GLFW_KEY_UP)) tr_x_camera.angles.x+=velocidade;
	
	if(input_pressed(GLFW_KEY_B)) alloc_new_box();
	if(input_pressing(GLFW_KEY_U)) textbox.font_size*=1.0f+time_delta();
	if(input_pressing(GLFW_KEY_J)) textbox.font_size*=1.0f-time_delta();

	if(input_pressed(GLFW_KEY_F3)) 
		graphics_set_debugmode(!graphics_get_debugmode());
	
	if(input_pressing(GLFW_KEY_O)) mat4_rotate_vec3(&suzanne.matrix,vec3_create(0,time_delta(),0));
	if(input_pressing(GLFW_KEY_L)) mat4_rotate_vec3(&suzanne.matrix,vec3_create(0,-time_delta(),0));

	if(input_pressed(GLFW_KEY_F5)) {
		printf("pau torando\n");
		scn_optimize(scn);
		//rlist_fst=rlist_optimize(rlist_fst,&bsp_root);
	}
/*

	Mat4 mov_matrix=MAT4_IDENTITY;
	mat4_translate_vec3(&mov_matrix,movimento);
	mat4_rotate_vec3(&mov_matrix,vec3_create(tr_x_camera.angles.x,tr_y_camera.angles.y,0));
	tr_base_camera.position=vec3_add(tr_base_camera.position,mat4_get_translation(mov_matrix));
*/

	//transform_into_view_matrix(tr_base_camera,&(main_camera.view_matrix));
	main_camera->view_matrix=
	mat4_mult(
		transform_to_view_matrix(tr_z_camera),
		mat4_mult(
			mat4_mult(
				transform_to_view_matrix(tr_x_camera),
				transform_to_view_matrix(tr_y_camera)
			),
			transform_to_view_matrix(tr_base_camera)
		)
	);
	/*
	
	*/
	//static bool cull_enabled=false;

	box.material.texture_scale=vec2_create(4,4);
	//renderer_draw(&box,main_camera);
	//renderer_draw(&suzanne,main_camera);

	//printf("node %d %d\n",bsp_root.left,bsp_root.right);
	//rlist_draw(rlist_fst,main_camera);
	//bsp_draw(&bsp_root,main_camera);
	scn_draw(scn,main_camera);
	aviao_update();
	renderer_draw(&rmap,main_camera);
	
	ui_draw_viewport();
	ui_rect_draw(&mira);
	
	painel.rect.position=vec2_create(
		((-50.0f)*window_aspect())+(painel.rect.size.x/2.0f)
		,(50.0f)-(painel.rect.size.y/2.0f)
	);
	textbox.rect=painel.rect;
	ui_rect_draw(&painel);
	char window_title[200];
	textbox.text=window_title;
	Vec3 cam_pos=tr_base_camera.position;
	snprintf(window_title,200,"FPS %d \n cam(%d,%d,%d) ",time_fps(),(int)cam_pos.x,(int)cam_pos.y,(int)cam_pos.z);
	textbox.text_length=strlen(window_title);
	ui_text_draw(&textbox);
}

void engine_destroy() {
	graphics_destroy();
	main_camera=camera_destroy(main_camera);
}

int main(int argc, char **argv) {
	if(!engine_init()) return -1;

	while(graphics_update()) {
		engine_update();
	}

	engine_destroy(); 
	return 0;
}
