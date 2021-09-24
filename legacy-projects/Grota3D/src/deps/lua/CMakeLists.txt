

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_STANDARD 99)

project(LUA VERSION 5.4 LANGUAGES C)


set(LUA_HEADERS 
    lopnames.h
    lopcodes.h
    lzio.h
    lparser.h
    lprefix.h
    lmem.h
    lstate.h
    llimits.h
    lstring.h
    llex.h
    ljumptab.h
    ltable.h
    lgc.h
    lfunc.h
    ltm.h
    ldo.h
    lua.h
    lapi.h
    ldebug.h
    luaconf.h
    lualib.h
    lctype.h
    lundump.h
    lcode.h
    lauxlib.h
    lvm.h
    lobject.h
)
set(LUA_SOURCES 
    lapi.c
    lzio.c
    lauxlib.c
    lvm.c
    lbaselib.c
    lcode.c
    lutf8lib.c
    lcorolib.c
    lctype.c
    lundump.c
    ldblib.c
    ldebug.c
    luac.c
    ldo.c
    lua.c
    ldump.c
    lfunc.c
    ltm.c
    lgc.c
    ltablib.c
    linit.c
    liolib.c
    ltable.c
    llex.c
    lstrlib.c
    lstring.c
    lmathlib.c
    lmem.c
    lstate.c
    loadlib.c
    lparser.c
    lobject.c
    lopcodes.c
    loslib.c
)


if (BUILD_SHARED_LIBS)
    add_library(lua SHARED ${LUA_SOURCES} ${LUA_HEADERS})
else()
    add_library(lua STATIC ${LUA_SOURCES} ${LUA_HEADERS})
endif()

