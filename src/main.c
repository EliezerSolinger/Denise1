#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>
#include "core/dmath.h"
#include "core/dsystem.h"


int main() {
	if(!window_init("MY FIRST GAME")) return false;
    while(window_update()) {
        if(input_pressed(GLFW_KEY_ESCAPE)) window_close();
    }
    window_destroy();
    return 0;
}

