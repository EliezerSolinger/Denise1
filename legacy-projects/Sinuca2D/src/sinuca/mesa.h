#ifndef SINUCA_MESA_H
#define SINUCA_MESA_H
#include "gameobject.h"
#include "objetos/bola.h"

typedef struct _Jogador{
    List* bolas_encacapadas;
} Jogador;

void mesa_init();
void mesa_update();
void mesa_destroy();


#endif