#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "core/mathematics.h"
#include "core/time.h"
#include "graphics/system/window.h"
#include "graphics/graphics.h"
#include "graphics/3d/camera.h"
#include "graphics/3d/renderer.h"
#include "graphics/ui/ui.h"
#include "graphics/3d/bsp.h"
#include "modules/obj_loader.h"
#include "modules/scripting.h"
#include <GLFW/glfw3.h>


int main(int argc, char **argv) {
    printf("initializing system...\n");
    if(!graphics_init(NULL)) return false;
	printf("OpenGL started.\n");
    printf("initializing scripting..\n");
    scripting_init();
    printf("Running!\n");
	while(graphics_update()) {
        scripting_update();
	}

    scripting_destroy();
	graphics_destroy();
	return 0;
}



