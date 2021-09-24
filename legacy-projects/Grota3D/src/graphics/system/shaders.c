
#include "shaders.h"
#include <stdio.h>

GLuint success;
char infoLog[512];

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
