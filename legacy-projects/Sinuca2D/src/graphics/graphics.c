#include <stdbool.h>
#include <stdio.h>
#include "core/time.h"
#include "graphics/2d/sprites.h"
#include "graphics/system/window.h"
#include "graphics/ui/ui.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "stb_image/stb_image.h"
/*
 * REQUIRE OPENGL 2.1 WITH ARB_VERTEX_ARRAY_OBJECT EXTENSION
 * 
 */

static int debugMode=false;
void graphics_set_debugmode(int value) {
	debugMode=value;
}
bool graphics_get_debugmode() {
	return debugMode;
}

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
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &data);
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &data);

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
bool graphics_init(const char *window_title,bool antialiasing) {
	if(!window_init(window_title,antialiasing)) return false;
	
 	load_null_texture();
 	load_blank_texture();
	ui_init();
	time_init();
	sprites_init();
	/*
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/
    return true;
}

bool graphics_update() {
	ui_update();	
	time_update();
	return window_update();
}
void graphics_destroy() {
	sprites_destroy();
	ui_destroy();
	window_destroy();
}
