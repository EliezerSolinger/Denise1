#ifndef GROTA_UI_H
#define GROTA_UI_H
#include <glad/glad.h>
#include "core/mathematics.h"
#include "core/colors.h"
typedef enum {
    Xleft,
    Xcenter,
    Xright
} XCorner;

typedef enum {
    Ytop,
    Ycenter,
    Ybottom
} YCorner;

typedef struct {
    Rect rect;
    XCorner corner_x;
    YCorner corner_y;
    Vec2 texture_scale;
    GLfloat rotation;
    Color4f color;
    bool black_as_alpha;
    GLuint texture_id;
} UIRect;
#define UIRECT_NEW { RECT_NEW,Xcenter,Ycenter,VEC2_ZERO,0,COLOR4F_WHITE,false,0}

typedef struct {
    Rect rect;
    char *text;
    GLuint text_length;
    GLfloat font_size;
    Color4f color;
    GLuint texture_font_id;
} UITextBox;

#define UITEXTBOX_NEW {RECT_NEW,"Sample Text\nThank you!!!",26,3.0f,COLOR4F_WHITE,0}

GLuint ui_quad_VAO_get();
void ui_init();
void ui_update();
void ui_destroy();
void ui_draw_viewport();

void ui_rect_draw(UIRect *rect);

void ui_text_draw(UITextBox *textbox);

#endif