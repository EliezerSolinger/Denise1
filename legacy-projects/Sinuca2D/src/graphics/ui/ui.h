#ifndef GROTA_UI_H
#define GROTA_UI_H
#include <glad/glad.h>
#include "core/mathematics.h"
#include "core/list.h"

typedef struct _UITextBox{
    Vec2 border;
    char text[256];
    Vec2 font_size;
    Vec2 font_adjust;
    Color4f color;
    bool use_shadow;
    Vec2 shadow_offset;
    Color4f shadow_color;
    GLuint *texture_font_id;
    bool hidden;
} UITextBox;


typedef struct _UIRect{
    Rect rect;
    XCorner corner_x;
    YCorner corner_y;
    Vec2 texture_scale;
    GLfloat rotation;
    Color4f color;
    bool use_shadow;
    Vec2 shadow_offset;
    Color4f shadow_color;
    bool black_as_alpha;
    GLuint *texture_id;
    bool hidden;
    bool stretch;
    UITextBox *textbox;
    List* child;
} UIRect;


#define UIRECT_NEW { RECT_NEW,Xcenter,Ycenter,VEC2_ZERO,0,COLOR4F_WHITE,false,VEC2_ZERO,COLOR4F_BLACK,false,NULL,true,false,NULL,NULL}

const static UIRect uirect_new=UIRECT_NEW;
UITextBox * ui_textbox_alloc();
void ui_textbox_destroy(UITextBox *txt);
void ui_textbox_setText(UITextBox *this,const char *text);
GLuint ui_quad_VAO_get();
void ui_init();
void ui_update();
void ui_destroy();
void ui_draw_viewport();

UIRect * ui_rect_alloc();
void ui_rect_destroy(UIRect *this);
void ui_rect_draw(UIRect *rect,Mat4* parent_matrix);
void ui_text_draw(UITextBox *textbox,Mat4 *rcmodel);

#endif