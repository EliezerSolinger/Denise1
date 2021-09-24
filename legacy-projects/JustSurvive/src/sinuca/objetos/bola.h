#ifndef BOLA_H
#define BOLA_H
#include "../gameobject.h"
#include <stdbool.h>

typedef struct _Bola{ 
    bool active;
    GameObject *object;
} Bola;

Bola* bola_carregar(int num);

void bola_ativar(Bola *this);
void bola_desativar(Bola *this);

void bola_atualizar(Bola *this);
void bola_destroy(Bola *this);



#endif