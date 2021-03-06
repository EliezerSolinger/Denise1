#include "dmath.h"
#include "dsystem.h"
#include "dgl.h"
#include "denise.h"
#include <stdlib.h>
#include <glad/glad.h>


/* AQUI COMECA AS COISAS DA ENGINE*/



Camera *camera_alloc() {
	Camera *self=malloc(sizeof(Camera));
	self->clear_color=color4f_create(0.6f,0.75f,1.0f,1.0f);
	self->is_perspective=true;
	self->fovy=70;
	self->aspect=1;
	self->left=50;
	self->right=-50;
	self->top=50;
	self->bottom=-50;
	self->zNear=0.1;
	self->zFar=1000;
	self->projection_matrix=MAT4_IDENTITY;
	self->view_matrix=MAT4_IDENTITY;
    self->viewport=RECT_EMPTY;
    self->render_layer=0;
	self->stats_drawcalls=0;
	self->stats_triangles_drawed=0;
    camera_update_projection_matrix(self);
	return self;
}
Camera *camera_destroy(Camera *camera) {
	free(camera);
	return NULL;
}
void camera_update_projection_matrix(Camera *camera) {
	if(camera->is_perspective) {
		camera->projection_matrix=
			mat4_perspective(camera->fovy,camera->aspect,camera->zNear,camera->zFar);
	} else {
		camera->projection_matrix=
			mat4_ortho(camera->left,camera->right,camera->bottom,camera->top,camera->zNear,camera->zFar);
	}
}
	

void camera_perspective(Camera *camera,float fovy,float aspect,float zNear,float zFar) {
	camera->fovy=fovy;
	camera->aspect=aspect;
	camera->zNear=zNear;
	camera->zFar=zFar;
	camera_update_projection_matrix(camera);
}

void camera_ortho(
			Camera *camera,
			float left,
			float right,
			float bottom,
			float top,
			float zNear,
			float zFar) {				
	camera->left=left;
	camera->right=right;
	camera->bottom=bottom;
	camera->top=top;
	camera->zNear=zNear;
	camera->zNear=zNear;
	camera_update_projection_matrix(camera);
}
/*
Mat4 camera_generate_shader_matrix(Camera *camera,Mat4 model) {
	//mat4_scale_vec3(&(camera->view_matrix),vec3_create(-1,-1,-1));
	return mat4_mult(
		camera->projection_matrix,
		mat4_mult(camera->view_matrix,model)
	);
}*/

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
	camera_update_projection_matrix(camera);
	//printf("Ajustando aspecto da camera xy para %f.",camera->aspect);
}




/* RENDERER */



#include "basic_meshes.h"



void mesh_freecontent(MeshData *mesh){
    free(mesh->vertices);
    mesh->vertices_count=0;
    mesh->vertices=NULL;
}



/// SHADERSSSSSSS
const char *renderer_vertex_shader= SHADER_INLINE(
    precision lowp float;
    precision lowp int;
    uniform mat4 model_matrix;
    uniform mat4 view_matrix;
    uniform mat4 projection_matrix;
    attribute vec3 a_vertex;
    attribute vec3 a_normal;
    attribute vec2 a_textcoord;
    varying vec2 texture_coordinates;
    void main() {
        gl_Position = (projection_matrix*view_matrix*model_matrix) * vec4(a_vertex, 1.0);
		texture_coordinates= vec2(a_textcoord.x, a_textcoord.y);
    }
    
);
   
    
const char *renderer_fragment_shader= SHADER_INLINE(
    precision lowp float;
    precision lowp int;
    uniform vec4 color;
    uniform vec2 texture_scale;
    uniform sampler2D texture;
    varying vec2 texture_coordinates;
    void main() {
        vec2 txc=texture_coordinates;
       /* if(texture_scale.x>0) {
            txc.x*=texture_scale.x;
            txc.y*=texture_scale.y;
        }*/
		gl_FragColor=color*texture2D(texture,txc);
        if(gl_FragColor.a<0.6) discard;
	}
);

   
static GLuint renderer_attribute_vertex;
static GLuint renderer_attribute_normal;
static GLuint renderer_attribute_textcoord;
static GLuint renderer_uniform_color;
static GLuint renderer_uniform_texture;
static GLuint renderer_uniform_model_matrix;
static GLuint renderer_uniform_view_matrix;
static GLuint renderer_uniform_projection_matrix;
static GLuint renderer_uniform_texture_scale;

static GLuint renderer_shader_program;


static bool renderer_shader_init() {
    
    static bool render_shader_initialized=false;
    if(!render_shader_initialized) {
        render_shader_initialized=true;
        renderer_shader_program=shader_create_program(renderer_vertex_shader,renderer_fragment_shader);
        if(renderer_shader_program==0) return false;  
        
        renderer_uniform_color=glGetUniformLocation(renderer_shader_program, "color");
        renderer_uniform_model_matrix=glGetUniformLocation(renderer_shader_program, "model_matrix");
        renderer_uniform_view_matrix=glGetUniformLocation(renderer_shader_program, "view_matrix");
        renderer_uniform_projection_matrix=glGetUniformLocation(renderer_shader_program, "projection_matrix");
        renderer_uniform_texture=glGetUniformLocation(renderer_shader_program, "texture");
        renderer_uniform_texture_scale=glGetUniformLocation(renderer_shader_program, "texture_scale");
        renderer_attribute_vertex=glGetAttribLocation(renderer_shader_program,"a_vertex");
        renderer_attribute_normal=glGetAttribLocation(renderer_shader_program,"a_normal");
        renderer_attribute_textcoord=glGetAttribLocation(renderer_shader_program,"a_textcoord");
    } 
    
    return true;
}

RenderObject mesh_load(MeshData *meshdata) {
    RenderObject obj={0};
        obj.vertices_count=meshdata->vertices_count;
    if(!renderer_shader_init()) return obj;
    glUniform1i(renderer_uniform_texture, 0);

    glGenBuffers(1, &(obj.VBO));

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    /*glBindVertexArray(obj.VAO);*/
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, meshdata->vertices_count*sizeof(VertexData), meshdata->vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(renderer_attribute_vertex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
    glEnableVertexAttribArray(renderer_attribute_vertex);
    // texture coord attribute
    
    glVertexAttribPointer(renderer_attribute_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(Vec3)));
    glEnableVertexAttribArray(renderer_attribute_normal);

    glVertexAttribPointer(renderer_attribute_textcoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(Vec3)*2));
    glEnableVertexAttribArray(renderer_attribute_textcoord);

    return obj;
}

void renderobject_unload(RenderObject *obj) {
    glDeleteBuffers(1, &(obj->VBO));
   obj->VBO=0;
}


static RenderObject debugbox_obj =RENDEROBJECT_NEW;
static Renderer debugbox_renderer=RENDERER_NEW;
static void debugbox_init() {
    static bool debugbox_initialized=false;
    if(!debugbox_initialized) {
        debugbox_initialized=true;
        MeshData meshboxdata={
            box_vertices,
            sizeof(box_vertices)/sizeof(VertexData)
        };
        debugbox_obj=mesh_load(&meshboxdata);
        debugbox_renderer.render_object=&debugbox_obj;
    }
    
}
static RenderObject debugsphere_obj = RENDEROBJECT_NEW;
static Renderer debugsphere_renderer=RENDERER_NEW;
static void debugsphere_init() {
    static bool debugsphere_initialized=false;
    if(!debugsphere_initialized) {
        debugsphere_initialized=true;
        MeshData meshspheredata={
            sphere_vertices,
            sizeof(sphere_vertices)/sizeof(VertexData)
        };
        debugsphere_obj=mesh_load(&meshspheredata);
        debugsphere_renderer.render_object=&debugsphere_obj;
    }
    
}
Renderer *renderer_debugbox() {
    debugbox_init();
	return &debugbox_renderer;
}
Renderer *renderer_debugsphere() {
    debugsphere_init();
	return &debugsphere_renderer;
}


void renderer_draw(Renderer *renderer,Camera *camera) {
  // render the renderer   
   /* if(!renderer_is_visible_on_camera(renderer,camera)) {
        return;
    }*/
    glUseProgram(renderer_shader_program); 
    
	if(renderer->material.albedo_texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderer->material.albedo_texture);
	} else {
        graphics_bind_null_texture();
    }
    
	/*Mat4 shader_matrix=camera_generate_shader_matrix(camera,renderer->matrix);*/
	glUniformMatrix4fv(renderer_uniform_model_matrix, 1, GL_FALSE, (GLfloat*)&(renderer->matrix));
	glUniformMatrix4fv(renderer_uniform_view_matrix, 1, GL_FALSE, (GLfloat*)&(camera->view_matrix));
	glUniformMatrix4fv(renderer_uniform_projection_matrix, 1, GL_FALSE, (GLfloat*)&(camera->projection_matrix));
    glUniform2fv(renderer_uniform_texture_scale,1, (GLfloat*) &(renderer->material.texture_scale));
    glUniform4fv(renderer_uniform_color,1, (GLfloat*) &(renderer->material.diffuse));

    
    glDrawArrays(GL_TRIANGLES,0,renderer->render_object->vertices_count);
    camera->stats_drawcalls++;
    camera->stats_triangles_drawed+=renderer->render_object->vertices_count;
}

