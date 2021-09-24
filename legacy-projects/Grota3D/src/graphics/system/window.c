
#include "core/mathematics.h"
#include "core/colors.h"
#include "core/time.h"
#include "window.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static bool running=false;
static GLFWwindow* window;
// INPUT
// estrutura para guardar os eventos do teclado
typedef struct {
    int action; // PRESSED,PRESING OU RELEASED
    int key; // GLFW_KEY_ESCAPE exemplo
} InputEvent;
/*
    cada frame ele guarda todos os eventos em um buffer assim essas teclas apertadas serão
    o estado da tecla no próximo frame
*/
// buffer para armazenar as teclas pressionadas
InputEvent events_buffer[INPUT_MAX_KEYS]={}; 
int input_event_buffer_count=0;

// buffer para armazenar os estados da tecla lido no frame anterior
InputEvent events[INPUT_MAX_KEYS]={};
int input_event_count=0;
/// copia do buffer para o estado e limpa o buffer
static void fx_input_update() {
    // copia do buffer para o estado
    memcpy(events,events_buffer,sizeof(InputEvent)*INPUT_MAX_KEYS);
    input_event_count=input_event_buffer_count;
    // limpa o buffer
    memset(events_buffer,0,sizeof(InputEvent)*INPUT_MAX_KEYS);
    input_event_buffer_count=0;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // guarda os eventos no buffer
    if(input_event_buffer_count<INPUT_MAX_KEYS) {
        events_buffer[input_event_buffer_count].key=key;
        if(action==GLFW_PRESS) {
            events_buffer[input_event_buffer_count].action=INPUT_PRESSED;
            input_event_buffer_count++;
        }
        else if(action==GLFW_RELEASE) {
            events_buffer[input_event_buffer_count].action=INPUT_RELEASED;
            input_event_buffer_count++;
        }
            
    }
}

int input_status(int key) {
    for (int i = 0; i < input_event_count; i++) {
        if(events[i].key==key) return events[i].action;
    }
    if(glfwGetKey(window,key)==GLFW_PRESS)
        return INPUT_PRESSING;
    return 0;
}

bool input_pressing(int key) {
	return input_status(key)==INPUT_PRESSING;
}
bool input_pressed(int key) {
	return input_status(key)==INPUT_PRESSED;
}
bool input_released(int key){
	return input_status(key)==INPUT_RELEASED;
}

//

static Vec2 window_size={800,600};
static bool window_show_fps=true;


int window_width(){ return (int)window_size.x;}
int window_height() { return (int)window_size.y;};
GLFWwindow *window_pointer() {
    return window;
}
void window_title(const char *window_title) {
    glfwSetWindowTitle(window,window_title);
}
float window_aspect() {
    return window_size.x/window_size.y;
}
bool window_init(const char *window_title) {
	 glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    int monitors_count;
    GLFWmonitor ** monitors=glfwGetMonitors(&monitors_count);
    static char default_window_title[10]= "GROTA 3D";
    if(!window_title) window_title=default_window_title;
        /// if you want full screen put monitors[0] on the fist null parameter    
    window = glfwCreateWindow((int)window_size.x, (int)window_size.y,window_title , NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if(!gladLoadGL()) {
        printf("Error Loading OpenGL With GLAD!\n");
        return 0;
    }
    glfwSwapInterval(1);
    running=true;
    return true;
}
void window_close() {
    running=false;
}
bool window_update() {
	//Sleep(10); 
	glfwSwapBuffers(window);
    glfwPollEvents();
    

    int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	window_size.x=(float)width;
	window_size.y=(float)height;
	if(window_show_fps) {
		//char window_title[100];
		//snprintf(window_title,100," FPS: %d",time_fps());
		//glutSetWindowTitle(window_title);
	}
	fx_input_update();
	return (!glfwWindowShouldClose(window) && running);
}

void window_destroy() {
	glfwDestroyWindow(window);
    glfwTerminate();
}



