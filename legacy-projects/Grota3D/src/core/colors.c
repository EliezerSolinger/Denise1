#include <glad/glad.h>
#include "colors.h"

Color3f color3f_create(GLfloat r,GLfloat g,GLfloat b) {
	Color3f result={r,g,b};
    return result;
}


Color4f color4f_create(GLfloat r,GLfloat g,GLfloat b,GLfloat a) {
	Color4f result={r,g,b,a};
    return result;
}
