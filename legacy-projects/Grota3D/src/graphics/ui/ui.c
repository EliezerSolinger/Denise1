#include "graphics/system/window.h" 
#include "core/mathematics.h"
#include "core/colors.h"
#include <glad/glad.h>

#include "graphics/system/shaders.h"
#include "graphics/graphics.h"

#include "ui.h"
#include <stdio.h>

/// SHADERSSSSSSS
const char *ui_vertex_shader= SHADER_INLINE(
    uniform mat4 matrix;
    attribute vec3 a_vertex;
    attribute vec2 a_textcoord;
    varying vec2 texture_coordinates;
    void main() {
        gl_Position = matrix * vec4(a_vertex, 1.0f);
		texture_coordinates= vec2(a_textcoord.x, a_textcoord.y);
    }
);
   
    
const char *ui_fragment_shader= SHADER_INLINE(
    uniform vec4 color;
    uniform bool black_as_alpha;
    uniform vec2 texture_scale;
    uniform vec2 texture_offset;
    uniform sampler2D texture;
    varying vec2 texture_coordinates;
    void main() {
        vec2 txc=texture_coordinates;
        if(texture_scale.x>0) {
            txc.x*=texture_scale.x;
            txc.y*=texture_scale.y;
        }
        txc+=texture_offset;
        gl_FragColor=color*texture2D(texture,txc);
        if(black_as_alpha) {
            if((gl_FragColor.r+gl_FragColor.g+gl_FragColor.b)<0.2) discard;
        }
        if(gl_FragColor.a<0.5) discard;
        
	}
);
   
static GLuint ui_attribute_vertex;
static GLuint ui_attribute_textcoord;
static GLuint ui_uniform_color;
static GLuint ui_uniform_texture;
static GLuint ui_uniform_matrix;
static GLuint ui_uniform_black_as_alpha;
static GLuint ui_uniform_texture_scale;
static GLuint ui_uniform_texture_offset;
static GLuint ui_shader_program;
GLuint ui_quad_VAO;
GLuint ui_quad_VAO_get() {
    return ui_quad_VAO;
}
GLuint ui_quad_VBO;


static bool ui_shader_init() {
    ui_shader_program=shader_create_program(ui_vertex_shader,ui_fragment_shader);
    if(ui_shader_program==0) return false;
    
    ui_uniform_color=glGetUniformLocation(ui_shader_program, "color");
    ui_uniform_matrix=glGetUniformLocation(ui_shader_program, "matrix");
    ui_uniform_texture=glGetUniformLocation(ui_shader_program, "texture");
    ui_uniform_black_as_alpha=glGetUniformLocation(ui_shader_program, "black_as_alpha");
    ui_uniform_texture_scale=glGetUniformLocation(ui_shader_program, "texture_scale");
    ui_uniform_texture_offset=glGetUniformLocation(ui_shader_program, "texture_offset");
    ui_attribute_vertex=glGetAttribLocation(ui_shader_program,"a_vertex");
    ui_attribute_textcoord=glGetAttribLocation(ui_shader_program,"a_textcoord");
    return true;
}


static bool ui_quad_vao_init() {  
     printf("chegou aq bb\n");
    GLfloat quad_data[] = {
        // positions      // texture coords
        0.5f,  0.5f,0.0f  , 1.0f, 1.0f, // top right
        0.5f, -0.5f,0.0f  , 1.0f, 0.0f, // bottom right
        -0.5f,  0.5f,0.0f  , 0.0f, 1.0f,  // top left 
        0.5f, -0.5f,0.0f  , 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,0.0f  , 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,0.0f  , 0.0f, 1.0f  // top left 
    };
    
    glUniform1i(ui_uniform_texture, 0);
    GLuint VAO,VBO;
    glGenVertexArrays(1, &VAO);
    ui_quad_VAO=VAO;
    glGenBuffers(1, &VBO);
    ui_quad_VBO=VBO;

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(ui_quad_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ui_quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, 6*(sizeof(Vec3)+sizeof(Vec2)), &quad_data, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(ui_attribute_vertex, 3, GL_FLOAT, GL_FALSE, (sizeof(Vec3)+sizeof(Vec2)), (void*)0);
    glEnableVertexAttribArray(ui_attribute_vertex);
    // texture coord attribute
    glVertexAttribPointer(ui_attribute_textcoord, 2, GL_FLOAT, GL_FALSE, (sizeof(Vec3)+sizeof(Vec2)), (void*)(sizeof(Vec3)));
    glEnableVertexAttribArray(ui_attribute_textcoord);
 	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
     glBindVertexArray(0);
}

static Mat4 ui_projection_matrix=MAT4_IDENTITY;

static Mat4 ui_generate_shader_matrix(Mat4 model) {
	return mat4_mult(
		ui_projection_matrix,
        model
	);
}

void ui_init(){
	ui_shader_init();
    ui_quad_vao_init();
}
void ui_update(){ 
    ui_projection_matrix=mat4_create(1);
    mat4_scale_vec3(
        &ui_projection_matrix,
        vec3_create(1.0f/(50.0f*window_aspect()),(1.0f/50.0f),1)
    );
}
void ui_draw_viewport() {
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    glViewport(0,0,window_width(),window_height());
}

void ui_destroy() {
    glDeleteVertexArrays(1, &ui_quad_VAO);
    glDeleteVertexArrays(1, &ui_quad_VBO);
}

static Vec2 ui_corner_translation(XCorner corner_x,YCorner corner_y) {
    Vec2 corner_translation=VEC2_ZERO;
    if(corner_x==Xleft) corner_translation.x=0.5f;
    if(corner_x==Xright) corner_translation.x=-0.5f;
    if(corner_y==Ybottom) corner_translation.y=0.5f;
    if(corner_y==Ytop) corner_translation.y=-0.5f;
    return corner_translation;
}
void ui_rect_draw(UIRect *rect) {
// render the renderer   

    glUseProgram(ui_shader_program); 
    
	if(rect->texture_id) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rect->texture_id);
	} else {
        graphics_bind_blank_texture();
    }
    Mat4 model=MAT4_IDENTITY;
    Rect rc=rect->rect;
    mat4_translate_vec2(&model,rc.position);

    mat4_scale_vec2(&model,rc.size);

    //mat4_translate_vec2(&model,ui_corner_translation(rect->corner_x,rect->corner_y));
    
	Mat4 shader_matrix=ui_generate_shader_matrix(model);
	glUniformMatrix4fv(ui_uniform_matrix, 1, GL_FALSE, (GLfloat*)&shader_matrix);
    glUniform2fv(ui_uniform_texture_scale,1, (GLfloat*) &(rect->texture_scale));
    glUniform1i(ui_uniform_black_as_alpha,rect->black_as_alpha);
    glUniform4fv(ui_uniform_color,1, (GLfloat*) &(rect->color));

    static const Vec2 empty_vec2=VEC2_ZERO;
    glUniform2fv(ui_uniform_texture_offset,1, (GLfloat*) &empty_vec2);

    glBindVertexArray(ui_quad_VAO);
    
    glDrawArrays(GL_TRIANGLES,0,6);
    
    glBindVertexArray(0);
}


void ui_text_draw(UITextBox *textbox){
    if(!textbox) {
        printf("trying to print a null pointer textbox\n");
        return;
    }    
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textbox->texture_font_id);

    // tem 16x16 caracteres na textura. entao 1/16
    static const GLfloat chscale=1.0f/16.0f;

    static Vec2 font_texture_scale={chscale,chscale};
    glUseProgram(ui_shader_program); 
    glUniform2fv(ui_uniform_texture_scale,1, (GLfloat*) &font_texture_scale);
    glUniform4fv(ui_uniform_color,1, (GLfloat*) &(textbox->color));

    Mat4 model=MAT4_IDENTITY;
    Rect rect=textbox->rect;
    mat4_translate_vec3(&model,vec3_create(rect.position.x-(rect.size.x/2.0f),rect.position.y+(rect.size.y/2.0f),0));
    
    glBindVertexArray(ui_quad_VAO);
    GLuint x=0,y=0;
    for(int c=0;c<textbox->text_length;c++) {
        char ascii_code=textbox->text[c];
        if(ascii_code==0) break;
        if((textbox->font_size*(float)x)>=rect.size.x) {
            x=0;y++;
        }
        if(ascii_code=='\n') {
            x=0;y++;continue;
        }
        if((textbox->font_size*(float)y)>=rect.size.y) {
            break;
        }
        //printf("%c\n %d",textbox->text[c],textbox->text[c]);
        Vec2 char_texture_offset=vec2_create((ascii_code%16)*chscale,(15-(ascii_code/16))*chscale);
        glUniform2fv(ui_uniform_texture_offset,1, (GLfloat*) &char_texture_offset);
        Mat4 charmat=MAT4_IDENTITY;
        mat4_translate_vec3(&charmat,vec3_create(textbox->font_size*(float)(x+0.5f),-(textbox->font_size*((float)y+0.5f)),0));
        mat4_scale_vec3(&charmat,vec3_create(textbox->font_size,textbox->font_size,0));
        
	    Mat4 shader_matrix=ui_generate_shader_matrix(mat4_mult(model,charmat));
        glUniformMatrix4fv(ui_uniform_matrix, 1, GL_FALSE, (GLfloat*)&shader_matrix);
        glDrawArrays(GL_TRIANGLES,0,6);
        x++;
    }
    
    glBindVertexArray(0);
}
