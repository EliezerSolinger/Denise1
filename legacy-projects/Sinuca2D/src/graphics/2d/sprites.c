#include "sprites.h"
#include "core/mathematics.h"
#include "camera2d.h"
#include "graphics/system/shaders.h"
#include "graphics/graphics.h"
#include <stdio.h>
#include <stdlib.h>


GLfloat quad_vertices[] = {
	// positions      // texture coords
	 0.5f,  0.5f,0.0f  , 1.0f, 1.0f, // top right
	 0.5f, -0.5f,0.0f  , 1.0f, 0.0f, // bottom right
	-0.5f,  0.5f,0.0f  , 0.0f, 1.0f, // top left 
     0.5f, -0.5f,0.0f  , 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f,0.0f  , 0.0f, 0.0f, // bottom left
	-0.5f,  0.5f,0.0f  , 0.0f, 1.0f  // top left 
};


/// SHADERSSSSSSS
const char *sprites_vertex_shader= SHADER_INLINE(
    uniform mat4 matrix;
    attribute vec3 a_vertex;
    attribute vec2 a_textcoord;
    varying vec2 texture_coordinates;
    void main() {
        gl_Position = matrix * vec4(a_vertex, 1.0f);
		texture_coordinates= vec2(a_textcoord.x, a_textcoord.y);
    }
);
   
    
const char *sprites_fragment_shader= SHADER_INLINE(
    uniform vec4 color;
    uniform vec2 texture_scale;
    uniform sampler2D texture;
    varying vec2 texture_coordinates;
    void main() {
        vec2 txc=texture_coordinates;
        if(texture_scale.x>0) {
            txc.x*=texture_scale.x;
            txc.y*=texture_scale.y;
        }
		gl_FragColor=color*texture2D(texture,txc);
        if(gl_FragColor.a<0.6) discard;
	}
);

static GLuint sprites_vao;
static GLuint sprites_vbo;
static GLuint sprites_attribute_vertex;
static GLuint sprites_attribute_textcoord;
static GLuint sprites_uniform_color;
static GLuint sprites_uniform_texture;
static GLuint sprites_uniform_matrix;
static GLuint sprites_uniform_texture_scale;
static GLuint sprites_shader_program;

bool sprites_init() {
    sprites_shader_program=shader_create_program(sprites_vertex_shader,sprites_fragment_shader);
    if(sprites_shader_program==0) return false;  
    
    sprites_uniform_color=glGetUniformLocation(sprites_shader_program, "color");
    sprites_uniform_matrix=glGetUniformLocation(sprites_shader_program, "matrix");
    sprites_uniform_texture=glGetUniformLocation(sprites_shader_program, "texture");
    sprites_uniform_texture_scale=glGetUniformLocation(sprites_shader_program, "texture_scale");
    sprites_attribute_vertex=glGetAttribLocation(sprites_shader_program,"a_vertex");
    sprites_attribute_textcoord=glGetAttribLocation(sprites_shader_program,"a_textcoord");

    glUniform1i(sprites_uniform_texture, 0);

    glGenVertexArrays(1, &sprites_vao);
    glGenBuffers(1, &(sprites_vbo));
  size_t vertexsize=(5*sizeof(GLfloat));
  	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(sprites_vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprites_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices),quad_vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(sprites_attribute_vertex, 3, GL_FLOAT, GL_FALSE, vertexsize, (void*)0);
    glEnableVertexAttribArray(sprites_attribute_vertex);
    // texture coord attribute
    
    glVertexAttribPointer(sprites_attribute_textcoord, 2, GL_FLOAT, GL_FALSE, vertexsize, (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(sprites_attribute_textcoord);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    return true;
}

void sprite_draw(Sprite *sprite,Camera2D *camera2d) {
    if(!sprite->visible) return;
    glUseProgram(sprites_shader_program); 
    
	if(sprite->texture_id) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *(sprite->texture_id));
	} else {
        graphics_bind_blank_texture();
    }
	Mat4 shader_matrix=camera2d_generate_shader_matrix(camera2d,transform2d_to_matrix(sprite->transform));
	glUniformMatrix4fv(sprites_uniform_matrix, 1, GL_FALSE, (GLfloat*)&shader_matrix);
    glUniform2fv(sprites_uniform_texture_scale,1, (GLfloat*) &(sprite->texture_scale));
    glUniform4fv(sprites_uniform_color,1, (GLfloat*) &(sprite->color));

    glBindVertexArray(sprites_vao);
    
    glDrawArrays(GL_TRIANGLES,0,sizeof(quad_vertices)/(5*sizeof(GLfloat)));
     
    glBindVertexArray(0);
    camera2d->stats_drawcalls++;
    camera2d->stats_triangles_drawed+=sizeof(quad_vertices)/sizeof(GLfloat);
}

void sprite_destroy(Sprite *this) {
    free(this);
}
void sprites_destroy() {
    glDeleteBuffers(1, &sprites_vao);
    glDeleteBuffers(1, &sprites_vbo);
}



Sprite* sprite_alloc() {
    Sprite *sprite=calloc(1,sizeof(Sprite));
    sprite->transform=transform2d_identity;
    sprite->visible=true;
    sprite->texture_scale=vec2_create(1.0f,1.0f);
    sprite->color=color4f_create(1.0f,1.0f,1.0f,1.0f);
    return sprite;
}