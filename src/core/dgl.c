#include "dmath.h"
#include "dsystem.h"
#include "dgl.h"
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

/* SHADERSS */
#define SHADER_INLINE(shader)  "#version 120\n" #shader


static GLuint success;
static char infoLog[512];

GLuint shader_compile_vertex(const char *shadersource) {
    printf("compiling vertex shader\n");
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &shadersource, NULL);
    glCompileShader(vertex_shader);
    // check for shader compile errors
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("%s,\n",infoLog);
    }
    return vertex_shader;
}

GLuint shader_compile_fragment(const char *shadersource) {
    printf("compiling vertex shader\n");
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &shadersource, NULL);
    glCompileShader(fragment_shader);
    // check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("%s,\n",infoLog);
        return 0;
    }
    return fragment_shader;
}

GLuint shader_link_program(GLuint vertex_shader,GLuint fragment_shader) {
    printf("linking shader program\n");
	GLuint shader_program=glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glUseProgram(shader_program);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        printf("%s,\n",infoLog);
        return 0;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    printf("shader %d linked!\n",shader_program);
    return shader_program;
}


GLuint shader_create_program(const char *vertex_shader,const char *fragment_shader) {
    GLuint vs=shader_compile_vertex(vertex_shader);
    if(!vs) return 0;
    GLuint fs=shader_compile_fragment(fragment_shader);
 
    if(!fs) return 0;
    return shader_link_program(vs,fs);
}

/* END SHADERS*/






#include <stdbool.h>
#include <stdio.h>
#include "dsystem.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "stb_image/stb_image.h"
/*
 * REQUIRE OPENGL 2.1 WITH ARB_VERTEX_ARRAY_OBJECT EXTENSION
 * 
 */

GLuint null_textureid=0;
GLuint blank_textureid=0;
static void load_null_texture() {
	GLuint data[4] = {
		0xFF00FFFF,0x000000FF,
		0x000000FF,0xFF00FFFF
	};
    glGenTextures(1, &null_textureid);
	glBindTexture(GL_TEXTURE_2D, null_textureid); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, &data);
}
static void load_blank_texture() {
	GLuint data[4] = {
		0xFFFFFFFF,0xFFFFFFFF,
		0xFFFFFFFF,0xFFFFFFFF
	};
    glGenTextures(1, &blank_textureid);
	glBindTexture(GL_TEXTURE_2D, blank_textureid); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, &data);

	glBindTexture(GL_TEXTURE_2D, null_textureid); 
}

void graphics_bind_null_texture() {
	glBindTexture(GL_TEXTURE_2D, null_textureid); 
}
void graphics_bind_blank_texture() {
	glBindTexture(GL_TEXTURE_2D, blank_textureid); 
}
GLuint *load_texture(const char *filename,bool linear) {
    GLuint *texture_id=calloc(1,sizeof(GLuint));
	printf("loading texture %s \n",filename);
   	 // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_set_flip_vertically_on_load(true);
    int width, height, nr_channels; 
	
	GLubyte *data = stbi_load(filename, &width, &height, &nr_channels, 4);
	  
   
	if(!data) {
		printf("texture not loaded, stb returned a null pointer.\n");
		return 0;
	}
    glGenTextures(1, texture_id);
  
	
    glBindTexture(GL_TEXTURE_2D, *texture_id); 
    
     // set the texture wrapping parameters
     // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
	if(linear) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
    // load image, create texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //  glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    // unbind texture to safe
    graphics_bind_null_texture();
	
	printf("texture loaded! id:%u info: %d x %d - %d channels \n",texture_id,width,height,nr_channels);
    
    return texture_id;
}

void texture_destroy(GLuint *texture_id) {
	glDeleteTextures(1,texture_id);
	free(texture_id);
}
bool graphics_init() {
 	load_null_texture();
 	load_blank_texture();
	/*
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/
    return true;
}


