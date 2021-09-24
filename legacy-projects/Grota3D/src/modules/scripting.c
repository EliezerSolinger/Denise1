#include <stdio.h>
#include <string.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include <lua/lapi.h>
#include <GLFW/glfw3.h>
#include "core/time.h"
#include "graphics/system/window.h"
#include "scripting.h"
#include "scripting/input.h"
//GLFW_KEY_F
lua_State *L; /* cria um novo estado Lua */
static int ldelta (lua_State *L) {
    lua_pushnumber (L, time_delta());
    return 1;
}
static int lelapsed (lua_State *L) {
    lua_pushnumber (L, time_elapsed());
    return 1;
}
static int linput (lua_State *L) {
    int key = luaL_checknumber (L, 1);
    lua_pushinteger (L, input_status(key));
    return 1;
}

static int lquit (lua_State *L) {
    window_close();
    return 0;
}


static const luaL_Reg script_functions [] = {
    {"elapsed", lelapsed},
    {"delta", ldelta},
    {"input", linput},
    {"quit",lquit},
    {NULL, NULL}
};

void inject_constants(const luaG_Reg *constants) {
     for(int c=0;true;c++) {
        if(constants[c].name==NULL) break;
        lua_pushinteger(L,constants[c].value);
        lua_setglobal(L,constants[c].name);
        //lua_pop(L,-1);
    }
}


int luaopen_mylib (lua_State *L) {
    inject_constants(script_input_constants);
    for(int c=0;c<sizeof(script_functions)/sizeof(luaL_Reg);c++) {
        if(script_functions[c].name==NULL) break;
         lua_register(L, script_functions[c].name, script_functions[c].func);
    }
    return 1;
}
void loadconfig(const char *file) {
    const char *color;
    if (luaL_loadfile (L, file) || lua_pcall (L, 0, 0, 0)) {
        printf ("error on load lua file: %s\n", lua_tostring (L, -1));
        return;
    }

}
void scripting_init() {
    L= luaL_newstate ();
    luaL_openlibs (L); /* da acesso a todas as bibliotecas padroes */
    loadconfig("game.lua");
        /* Coloca o valor da variavel no topo da pilha */
    lua_getglobal (L, "window_title");

    if (!lua_isstring (L, -1)) {
        printf("using default window title.\n");
    } else {
        window_title(lua_tolstring (L, -1, NULL));
    }
    lua_pop(L,-1);
    
    lua_getglobal (L, "init");
    if (lua_pcall (L, 0, 0, 0) != 0) {
        printf ("Erro chamando gameinit: %s\n", lua_tostring (L, -1));
    }
    lua_pop(L,-1);
    luaopen_mylib(L);
  
   
}

void scripting_update() {
    lua_getglobal (L, "update");
    if (lua_pcall (L, 0, 0, 0) != 0) {
        printf ("Erro chamando update: %s\n", lua_tostring (L, -1));
    }
    lua_pop(L,-1);
  
}
void scripting_destroy() {
    lua_getglobal (L, "destroy");
    if (lua_pcall (L, 0, 0, 0) != 0) {
        printf ("Erro chamando destroy: %s\n", lua_tostring (L, -1));
    }
    lua_pop(L,-1);
  
    lua_close (L);    
}