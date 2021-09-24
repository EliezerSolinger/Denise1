#include "gui.h"
#include "graphics/ui/ui.h"
#include "graphics/graphics.h"
#include "stdlib.h"
#include "core/time.h"
#include "modules/uixml.h"

UIRect *alerta;

UIRect *barraforca_fundo;
UIRect *barraforca;
float tempo_alerta=0;

UIXml *xml;
void mostrar_alerta(const char* text,float tempo) {
    ui_textbox_setText(alerta->textbox,text);
    tempo_alerta=tempo;
}
void gui_carregar() {
    xml=uixml_load("ui/mesa.xml");
    alerta=uixml_getbyid(xml,"alerta");

    barraforca_fundo=uixml_getbyid(xml,"barraforca_fundo");
    barraforca=uixml_getbyid(xml,"barraforca");
}
void gui_atualizar(float forca_da_barra){

    //printf("tim al %f\n",tempo_alerta);
    barraforca_fundo->hidden=forca_da_barra<EQUAL_EPSILON;

    barraforca->rect.size.y=forca_da_barra;
    bool mostrando_alerta=tempo_alerta>EQUAL_EPSILON;
    if(mostrando_alerta) {
        tempo_alerta-=time_delta();
    }
    alerta->hidden=!mostrando_alerta;

    ui_draw_viewport();
    uixml_draw(xml);

}
void gui_destruir(){
    uixml_destroy(xml);
}
