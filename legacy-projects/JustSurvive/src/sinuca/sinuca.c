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
#include "modules/uixml.h"

int main() {
    time_init();
    if(!graphics_init("SINUCA 2D 0.0.0a",false)) {
        printf("error to init graphics engine!\n");
    };
    mesa_init();

    while (graphics_update()){
        if(input_pressed(GLFW_KEY_F12)) {
            mesa_destroy();
            mesa_init();
        }
        if(input_pressed(GLFW_KEY_F11))
             glfwMaximizeWindow(window_pointer());
    
        mesa_update();
    }
    mesa_destroy();
    graphics_destroy();
    return EXIT_SUCCESS;
}