#ifndef GROTA_COLORS_H
#define GROTA_COLORS_H

#include <glad/glad.h>

typedef struct Color3f{
    GLfloat r, g, b;
} Color3f;
#define COLOR3F_WHITE {1.0f, 1.0f ,1.0f}
#define COLOR3F_BLACK {0.0f, 0.0f, 0.0f}
#define COLOR3F_RED  {1.0f, 0.0f, 0.0f}
#define COLOR3F_GREEN {0.0f, 1.0f, 0.0f}
#define COLOR3F_BLUE {0.0f, 0.0f, 1.0f}

typedef struct Color4f{
    GLfloat r, g, b, a;
} Color4f;
#define COLOR4F_WHITE {1.0f, 1.0f ,1.0f, 1.0f}
#define COLOR4F_BLACK {0.0f, 0.0f, 0.0f, 1.0f}
#define COLOR4F_RED  {1.0f, 0.0f, 0.0f, 1.0f}
#define COLOR4F_GREEN {0.0f, 1.0f, 0.0f, 1.0f}
#define COLOR4F_BLUE {0.0f, 0.0f, 1.0f, 1.0f}

Color3f color3f_create(GLfloat r,GLfloat g,GLfloat b);
Color4f color4f_create(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
#endif




