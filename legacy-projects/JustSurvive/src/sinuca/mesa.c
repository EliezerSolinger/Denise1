#include "mesa.h"

#include "core/mathematics.h"
#include "core/time.h"
#include "core/list.h"
#include "graphics/ui/ui.h"
#include "graphics/graphics.h"
#include "graphics/2d/camera2d.h"
#include "graphics/2d/sprites.h"
#include "graphics/system/window.h"
#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "physics/physics2d.h"
#include "gameobject.h"
#include "mesa.h"
#include "gui.h"

PhysicsWorld *phx=NULL;

Camera2D *camera2d;
Sprite *piso;
Sprite *mesa;
Sprite *taco;
Sprite *buracos[6]={0};
Transform2D tr_taco;
Transform2D tr_taco_x;
GameObject *laterais[6]={0};
GLuint *textura_buraco;


float forca_do_taco=0;
static float forca_maxima=50;

void desenhar_sprite_com_sombra(Sprite *sprite,Camera2D *camera2d,Color4f shadow_color,float offset) {
    Color4f original_color=sprite->color;
    sprite->color=shadow_color;
    sprite->transform.position.x-=offset;
    sprite->transform.position.y-=offset;
    sprite_draw(sprite,camera2d);
    sprite->transform.position.x+=offset;
    sprite->transform.position.y+=offset;
    sprite->color=original_color;
    sprite_draw(sprite,camera2d);
} 



void carregar_mesa() {
    
    piso=sprite_alloc();
    piso->texture_id=load_texture("brick.png",true);
    piso->transform.size=vec2_create(2048,2048);
    piso->texture_scale=vec2_create(1024/48,1024/48);

    mesa=sprite_alloc();
    mesa->texture_id=load_texture("mesa.png",false);
    mesa->transform.size=vec2_create(523,292);
    taco=sprite_alloc();
    taco->texture_id=load_texture("taco.png",false);
    taco->transform.size=vec2_create(256,32);

    const static float buraco=45;
    const static float borda=20;

    laterais[0]=gameobject_alloc();
    laterais[0]->sprite->transform.size.x=(mesa->transform.size.x/2.0f)-buraco;
    laterais[0]->sprite->transform.size.y=borda;    
    laterais[0]->sprite->transform.position=vec2_create(-(mesa->transform.size.x/4)+5,-(mesa->transform.size.y/2));

    laterais[1]=gameobject_alloc();
    laterais[1]->sprite->transform.size.x=(mesa->transform.size.x/2.0f)-buraco;
    laterais[1]->sprite->transform.size.y=borda;    
    laterais[1]->sprite->transform.position=vec2_create((mesa->transform.size.x/4)-5,-(mesa->transform.size.y/2));

    laterais[2]=gameobject_alloc();
    laterais[2]->sprite->transform.size.x=(mesa->transform.size.x/2.0f)-buraco;
    laterais[2]->sprite->transform.size.y=borda;    
    laterais[2]->sprite->transform.position=vec2_create(-(mesa->transform.size.x/4)+5,(mesa->transform.size.y/2));

    laterais[3]=gameobject_alloc();
    laterais[3]->sprite->transform.size.x=(mesa->transform.size.x/2.0f)-buraco;
    laterais[3]->sprite->transform.size.y=borda;    
    laterais[3]->sprite->transform.position=vec2_create((mesa->transform.size.x/4)-5,(mesa->transform.size.y/2));

    laterais[4]=gameobject_alloc();
    laterais[4]->sprite->transform.size.y=mesa->transform.size.y-buraco-5;
    laterais[4]->sprite->transform.size.x=borda;    
    laterais[4]->sprite->transform.position=vec2_create(-(mesa->transform.size.x/2),0);

    laterais[5]=gameobject_alloc();
    laterais[5]->sprite->transform.size.y=mesa->transform.size.y-buraco-5;
    laterais[5]->sprite->transform.size.x=borda;    
    laterais[5]->sprite->transform.position=vec2_create((mesa->transform.size.x/2),0);

    for(int c=0;c<6;c++) {
        laterais[c]->sprite->color=color4f_create(0.5f,0.3f,0.2f,1.0f);
        body_resize(laterais[c]->body,laterais[c]->sprite->transform.size,0);
        laterais[c]->body->position=laterais[c]->sprite->transform.position;
        laterais[c]->body->bouncy=0.5f;
        //laterais[c].body->static_friction=0.2f;
        printf(" phx %d, lateral %d\n",phx,laterais[c]->body);
        physics_add_body(phx,laterais[c]->body);
    }
    textura_buraco=load_texture("buraco.png",false);
    for(int c=0;c<6;c++) {
        buracos[c]=sprite_alloc();
        buracos[c]->texture_id=textura_buraco;
        buracos[c]->transform.size=vec2_create(48,48);    
    } 
    Vec2 halfmesa=vec2_scalar_mult(mesa->transform.size,0.5f);
    buracos[0]->transform.position=
        vec2_create(-halfmesa.x,-halfmesa.y);
    buracos[1]->transform.position=
        vec2_create(-halfmesa.x,halfmesa.y);
    buracos[2]->transform.position=
        vec2_create(halfmesa.x,-halfmesa.y);
    buracos[3]->transform.position=
        vec2_create(halfmesa.x,halfmesa.y);
    halfmesa.y+=5;
    halfmesa.y+=5;
    
    buracos[4]->transform.position=
        vec2_create(0,-halfmesa.y);
    buracos[5]->transform.position=
        vec2_create(0,halfmesa.y);
   
}


Bola *bolas[16]={0};
void posicionar_bolas() {
    tr_taco.rotation=(randf()*0.2f)-0.1f;
    float ball_distance=16+5;
    float x_distance=70;
    int bola=1;
    body_stop(bolas[0]->object->body);
    bola_ativar(bolas[0]);
    bolas[0]->object->body->position=vec2_create(randf()*2,randf()*2);
    for(int x=0;x<5;x++) {
        for(int y=0;y<=x;y++) {
            int bolatual=bola++;            
            bola_ativar(bolas[bolatual]);
            Body *body=bolas[bolatual]->object->body;
            body_stop(body);
            body->active=true;
            body->position=vec2_create(
                    (x*ball_distance)+x_distance+(randf()*3)+((randf()-0.5f)*3),
                    (y*ball_distance)-((x*ball_distance)/2)+((randf()-0.5f)*3)
                );
        }
    }
}
void carregar_bolas() {
    for(int c=0;c<16;c++) {
        bolas[c]=bola_carregar(c);
        Bola *b=bolas[c];
        physics_add_body(phx,b->object->body);
    }
    posicionar_bolas();
}
bool bola_branca_esta_parada() {
    return vec2_magnitude(bolas[0]->object->body->velocity)<5.f;
}
void bola_encapada(int bola);
void atualizar_bolas() {
    for(int c=0;c<16;c++) {
        if(!(bolas[c]->active)) continue;
        bola_atualizar(bolas[c]);

        Vec2 posicao=bolas[c]->object->body->position; 
        bool escapou=fabsf(posicao.x)>mesa->transform.size.x/2 || fabsf(posicao.y)>mesa->transform.size.y/2;
        bool caiu_no_buraco=false;
        for(int i=0;i<sizeof(buracos)/sizeof(void*);i++) {
            float distancia=vec2_distance(buracos[i]->transform.position,posicao);
            if(distancia<(buracos[i]->transform.size.x/2.f)-(bolas[c]->object->body->size.x/2.0f)) {
                caiu_no_buraco=true;
                break;
            }
        }
        if(escapou || caiu_no_buraco) {
            printf("BOLA %d FOI ENCACAPADA!!!\n",c);
            bola_encapada(c);
            //posicionar_bolas();
        } 
    }
    for(int c=0;c<4;c++) {
        gameobject_update(laterais[c]);
    }
    tr_taco.position=bolas[0]->object->body->position;
}

Jogador jogadores[2]={0};
int jogador_atual=0;
void bola_encapada(int bola) {
    bola_desativar(bolas[bola]);
}
void jogadores_carregar() {
    mostrar_alerta("Jogador 1\n tua vez!!",3);
}
void jogadores_atualizar() {
}
void jogadores_desenhar_ui() {
    static UITextBox txt1;
}
void jogadores_destruir() {

}
void mesa_init() {
    randf();randf();
    camera2d=camera2d_alloc();
    phx=physics_alloc(32);
    phx->gravity=vec2_create(0,0);
    //phx->air_density=3;
    carregar_bolas();
    
    carregar_mesa();

    camera2d->zoom=350;
    gui_carregar();
    jogadores_carregar();

    
}
void mesa_update() {

    ///  LOGICA
    
    tr_taco.position=vec2_lerp(tr_taco.position,bolas[0]->object->body->position,time_delta()*3);
    tr_taco_x.position.x=-128-8-forca_do_taco;
    
    Mat4 mat_taco=mat4_identity;
    mat4_translate_vec2(&mat_taco,tr_taco_x.position);
    mat4_rotate(&mat_taco,tr_taco.rotation);
    mat4_translate_vec2(&mat_taco,tr_taco.position);
    taco->transform.position=mat4_get_translation(mat_taco);
    taco->transform.rotation=tr_taco.rotation;
    // camera2d->rotation=lerp(camera2d->rotation,taco->transform.rotation+(3.1415/2.0f),time_delta()*2);
    atualizar_bolas();
    physics_update(phx,time_delta(),200);
    float velocidade=time_delta()*40.0f;
    if(input_pressing(GLFW_KEY_KP_ADD) || input_pressing(GLFW_KEY_EQUAL)) {
        camera2d->zoom-=velocidade;
    }
    if(input_pressing(GLFW_KEY_MINUS) || input_pressing(GLFW_KEY_KP_SUBTRACT)) {
        camera2d->zoom+=velocidade;
    }
    if(input_pressing(GLFW_KEY_W)) camera2d->position.y+=velocidade;
    if(input_pressing(GLFW_KEY_S)) camera2d->position.y-=velocidade;

    if(input_pressing(GLFW_KEY_D)) camera2d->position.x+=velocidade;
    if(input_pressing(GLFW_KEY_A)) camera2d->position.x-=velocidade;
    camera2d->position=vec2_lerp(camera2d->position,bolas[0]->object->body->position,time_delta()*2);
    Vec2 impulse=vec2_create(0,0);

    if(input_pressed(GLFW_KEY_F5)) posicionar_bolas();
     if(bola_branca_esta_parada()) {
        if(input_pressing(GLFW_KEY_LEFT)) tr_taco.rotation-=time_delta()*2;
        if(input_pressing(GLFW_KEY_RIGHT)) tr_taco.rotation+=time_delta()*2;
        static float forca_lancamento=0;
        static bool lancando=false;
        if(input_pressing(GLFW_KEY_SPACE)) {
            forca_do_taco=lerp(forca_do_taco,forca_maxima,time_delta());
            forca_lancamento=forca_do_taco;
        }
        if(input_released(GLFW_KEY_SPACE)) {
            lancando=true;
        }
        if(lancando) {
            forca_do_taco-=time_delta()*300;
            if(forca_do_taco<=0) {
                printf("forca_lancamento %f\n",forca_lancamento);
                Mat4 t=mat4_create(1.0f);
                mat4_translate_vec2(&t,vec2_create(forca_lancamento*1000,0));
                mat4_rotate(&t,tr_taco.rotation);
                body_apply_impulse(bolas[0]->object->body,mat4_get_translation(t));
                lancando=false;
                forca_do_taco=0;
                forca_lancamento=0;
            }
        }
    }
    ///// DRAWWWW DESENHAR
    camera2d_fix_aspect(camera2d);
    camera2d_draw_viewport(camera2d);
    sprite_draw(piso,camera2d);
    sprite_draw(mesa,camera2d);
    for(int c=0;c<6;c++) sprite_draw(buracos[c],camera2d);
    for(int c=0;c<6;c++) desenhar_sprite_com_sombra(laterais[c]->sprite,camera2d,color4f_create(0,0.2f,0,1.0f),2);
    for(int c=0;c<sizeof(bolas)/sizeof(void*);c++) {
        desenhar_sprite_com_sombra(bolas[c]->object->sprite,camera2d,color4f_create(0,0.2f,0,1.0f),2);
    }
    if(bola_branca_esta_parada()) {
        desenhar_sprite_com_sombra(taco,camera2d,color4f_create(0,0,0,1),2);
    } 
    gui_atualizar(forca_do_taco/forca_maxima);
    jogadores_atualizar();
}

void mesa_destroy() {
    gui_destruir();
    jogadores_destruir();
    texture_destroy(mesa->texture_id);
    texture_destroy(taco->texture_id);
    texture_destroy(piso->texture_id);
    texture_destroy(textura_buraco);
    sprite_destroy(piso);
    sprite_destroy(mesa);
    sprite_destroy(taco);
    for(int c=0;c<sizeof(bolas)/sizeof(void*);c++) {
        bola_destroy(bolas[c]);
    }
    for(int c=0;c<sizeof(buracos)/sizeof(void*);c++) {
        sprite_destroy(buracos[c]);
    }
    for(int c=0;c<sizeof(laterais)/sizeof(void*);c++) {
        gameobject_destroy(laterais[c]);
    }
    camera2d_destroy(camera2d);
    physics_destroy(phx);
}