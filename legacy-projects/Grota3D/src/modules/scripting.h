#ifndef GROTA_SCRIPTING_MODULE_H
#define GROTA_SCRIPTING_MODULE_H
typedef struct luaG_Reg {
  const char *name;
  int value;
} luaG_Reg;

void scripting_init();
void scripting_update();
void scripting_destroy();

#endif