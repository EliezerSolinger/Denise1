#include "dmath.h"
#include "dsystem.h"
#include "dgl.h"
#include "denise.h"
#include <stdlib.h>
#include <glad/glad.h>


/* AQUI COMECA AS COISAS DA ENGINE*/



Camera *camera_alloc() {
	Camera *this=malloc(sizeof(Camera));
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
	this->projection_matrix=MAT4_IDENTITY;
	this->view_matrix=MAT4_IDENTITY;
    this->viewport=RECT_EMPTY;
    this->render_layer=0;
	this->stats_drawcalls=0;
	this->stats_triangles_drawed=0;
    camera_update_projection_matrix(this);
	return this;
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

Mat4 camera_generate_shader_matrix(Camera *camera,Mat4 model) {
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
    uniform mat4 matrix;
    attribute vec3 a_vertex;
    attribute vec3 a_normal;
    attribute vec2 a_textcoord;
    varying vec2 texture_coordinates;
    void main() {
        gl_Position = matrix * vec4(a_vertex, 1.0);
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

const char* renderer_phong_vertex_shader= SHADER_INLINE(
    precision mediump float;
    precision mediump int;
    attribute vec3 position;
    attribute vec3 normal;
    uniform mat4 projection;
    uniform mat4 modelview;
    uniform mat4 normalMat;
    varying vec3 normalInterp;
    varying vec3 vertPos;

    void main(){
        vec4 vertPos4 = modelview * vec4(position, 1.0);
        vertPos = vec3(vertPos4) / vertPos4.w;
        normalInterp = vec3(normalMat * vec4(normal, 0.0));
        gl_Position = projection * vertPos4;
    }
);
const char* renderer_phong_fragment_shader= SHADER_INLINE(
    precision mediump float;
    precision mediump int;
    precision mediump float;
    varying vec3 normalInterp;  // Surface normal
    varying vec3 vertPos;       // Vertex position
    uniform int mode;   // Rendering mode
    uniform float Ka;   // Ambient reflection coefficient
    uniform float Kd;   // Diffuse reflection coefficient
    uniform float Ks;   // Specular reflection coefficient
    uniform float shininessVal; // Shininess
    // Material color
    uniform vec3 ambientColor;
    uniform vec3 diffuseColor;
    uniform vec3 specularColor;
    uniform vec3 lightPos; // Light position

    void main() {
    vec3 N = normalize(normalInterp);
    vec3 L = normalize(lightPos - vertPos);

    // Lambert's cosine law
    float lambertian = max(dot(N, L), 0.0);
    float specular = 0.0;
    if(lambertian > 0.0) {
        vec3 R = reflect(-L, N);      // Reflected light vector
        vec3 V = normalize(-vertPos); // Vector to viewer
        // Compute the specular term
        float specAngle = max(dot(R, V), 0.0);
        specular = pow(specAngle, shininessVal);
    }
    gl_FragColor = vec4(Ka * ambientColor +
                        Kd * lambertian * diffuseColor +
                        Ks * specular * specularColor, 1.0);

    // only ambient
    if(mode == 2) gl_FragColor = vec4(Ka * ambientColor, 1.0);
    // only diffuse
    if(mode == 3) gl_FragColor = vec4(Kd * lambertian * diffuseColor, 1.0);
    // only specular
    if(mode == 4) gl_FragColor = vec4(Ks * specular * specularColor, 1.0);
    }
);
   
static GLuint renderer_attribute_vertex;
static GLuint renderer_attribute_normal;
static GLuint renderer_attribute_textcoord;
static GLuint renderer_uniform_color;
static GLuint renderer_uniform_texture;
static GLuint renderer_uniform_matrix;
static GLuint renderer_uniform_texture_scale;
static GLuint renderer_shader_program;


static bool renderer_shader_init() {
    
    static bool render_shader_initialized=false;
    if(!render_shader_initialized) {
        render_shader_initialized=true;
        renderer_shader_program=shader_create_program(renderer_vertex_shader,renderer_fragment_shader);
        if(renderer_shader_program==0) return false;  
        
        renderer_uniform_color=glGetUniformLocation(renderer_shader_program, "color");
        renderer_uniform_matrix=glGetUniformLocation(renderer_shader_program, "matrix");
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

   /* glGenVertexArrays(1, &(obj.VAO));*/
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

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
   /* glBindVertexArray(0);*/

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

void debugbox_draw(Camera *camera,Mat4 matrix,Color4f color) {
    debugbox_init();
    graphics_bind_blank_texture();
    GLboolean cull_status;
    glGetBooleanv(GL_CULL_FACE,&cull_status);
    glDisable(GL_CULL_FACE);
   /* glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);*/
  // render the renderer   
    glUseProgram(renderer_shader_program); 
    
    glActiveTexture(GL_TEXTURE0);
	
	Mat4 shader_matrix=camera_generate_shader_matrix(camera,matrix);
	glUniformMatrix4fv(renderer_uniform_matrix, 1, GL_FALSE, (GLfloat*)&shader_matrix);
    static Vec2 tmp_texture_scale=VEC2_FILL_ONE;
    //static Color4f tmp_color=COLOR4F_RED;
    glUniform2fv(renderer_uniform_texture_scale,1, (GLfloat*) &(tmp_texture_scale));
    glUniform4fv(renderer_uniform_color,1, (GLfloat*) &(color));
    /*glBindVertexArray(debugbox_obj.VAO);*/

    glDrawArrays(GL_TRIANGLES,0,debugbox_obj.vertices_count);

    
   /* glBindVertexArray(0);*/
  /*  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);*/
    if(cull_status) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void debugsphere_draw(Camera *camera,Mat4 matrix,Color4f color) {
    debugsphere_init();
    graphics_bind_blank_texture();
    GLboolean cull_status;
    glGetBooleanv(GL_CULL_FACE,&cull_status);
    glDisable(GL_CULL_FACE);
    /*glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);*/
  // render the renderer   
    glUseProgram(renderer_shader_program); 
    
    glActiveTexture(GL_TEXTURE0);
	
	Mat4 shader_matrix=camera_generate_shader_matrix(camera,matrix);
	glUniformMatrix4fv(renderer_uniform_matrix, 1, GL_FALSE, (GLfloat*)&shader_matrix);
    static Vec2 tmp_texture_scale=VEC2_FILL_ONE;
    //static Color4f tmp_color=COLOR4F_RED;
    glUniform2fv(renderer_uniform_texture_scale,1, (GLfloat*) &(tmp_texture_scale));
    glUniform4fv(renderer_uniform_color,1, (GLfloat*) &(color));
    /*glBindVertexArray(debugsphere_obj.VAO);*/

    glDrawArrays(GL_TRIANGLES,0,debugsphere_obj.vertices_count);

    
   /* glBindVertexArray(0);*/
    /*glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);*/
    if(cull_status) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
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
    
	Mat4 shader_matrix=camera_generate_shader_matrix(camera,renderer->matrix);
	glUniformMatrix4fv(renderer_uniform_matrix, 1, GL_FALSE, (GLfloat*)&shader_matrix);
    glUniform2fv(renderer_uniform_texture_scale,1, (GLfloat*) &(renderer->material.texture_scale));
    glUniform4fv(renderer_uniform_color,1, (GLfloat*) &(renderer->material.diffuse));

    
    glDrawArrays(GL_TRIANGLES,0,renderer->render_object->vertices_count);
    camera->stats_drawcalls++;
    camera->stats_triangles_drawed+=renderer->render_object->vertices_count;
}


void renderer_init_quad(Renderer *quad) {
    static MeshData qmesh={0,0};
    static RenderObject quad_obj=RENDEROBJECT_NEW;
    if(qmesh.vertices_count==0) {
        qmesh.vertices=quad_vertices;
        qmesh.vertices_count=sizeof(quad_vertices)/sizeof(VertexData);
        quad_obj=mesh_load(&qmesh);
    }
    quad->render_object=&quad_obj;
}
void renderer_init_box(Renderer *box) {
    static MeshData bmesh={0,0};
    static RenderObject box_obj=RENDEROBJECT_NEW;
    if(bmesh.vertices_count==0) {
        bmesh.vertices=box_vertices;
        bmesh.vertices_count=sizeof(box_vertices)/sizeof(VertexData);
        box_obj=mesh_load(&bmesh);
    }
    box->render_object=&box_obj;
}
