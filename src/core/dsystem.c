
#include "dmath.h"
#include "dsystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GLFW/glfw3.h>

#include <glad/glad.h>

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

static int window_size[2]={800,600};
static bool window_show_fps=true;

int window_width(){ return window_size[0];}
int window_height() { return window_size[1];};

float window_aspect() {
    return (float)window_width()/(float)window_height();
}
bool window_init(const char *window_title) {
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);

    /*if(antialiasing) glfwWindowHint(GLFW_SAMPLES, 4);*/
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    int monitors_count;
    GLFWmonitor ** monitors=glfwGetMonitors(&monitors_count);
    static char default_window_title[10]= "GROTA 3D";
    if(!window_title) window_title=default_window_title;
        /// if you want full screen put monitors[0] on the fist null parameter    
    window = glfwCreateWindow(window_width(),window_height(),window_title , NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
        printf("Error Loading OpenGL With GLAD\r\n");
        return -1;
    }    

/*
	gladLoadGLESLoader((GLADloadproc)glfwGetProcAddress);
	if(!gladLoadGL()) {
        printf("Error Loading OpenGL With GLAD!\n");
        return 0;
    }*/
    /*if(antialiasing) glEnable(GL_MULTISAMPLE); */
    glfwSwapInterval(1);
    running=true;
    time_init();
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
	window_size[0]=width;
	window_size[1]=height;
	if(window_show_fps) {
		//char window_title[100];
		//snprintf(window_title,100," FPS: %d",time_fps());
		//glutSetWindowTitle(window_title);
	}
	fx_input_update();
    time_update();
	return (!glfwWindowShouldClose(window) && running);
}

void window_destroy() {
	glfwDestroyWindow(window);
    glfwTerminate();
}



#include <stdlib.h>
#include <sys/time.h>

static long initial_time=0;
static long last_update_time=0;
static float delta=0;

static float delta_average=0;
/**
 * Returns the current time in microseconds using posix function,
 */

static inline long getSysMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

void time_init() {
	initial_time=getSysMicrotime();
	last_update_time=initial_time;
}
float time_elapsed() {
	return (float) (getSysMicrotime()-initial_time)/1e6f;
};
float time_elapsed_since(float time_point) {
	return time_elapsed()-time_point;
}

long time_elapsed_millis() {
	return (getSysMicrotime()-initial_time)/1000;
}
long time_elapsed_micro(){
	return getSysMicrotime()-initial_time;
};

float time_average_delta() {
	return delta_average;
}
float time_delta() {
	return delta;
}
void time_update(){
	long now_micro=getSysMicrotime();

	delta=(float)((now_micro-last_update_time)/1e6f);
	last_update_time=now_micro;
	
	if(delta_average==0) {
		delta_average=delta;
	} else {
		// linear interpolation
		delta_average=delta_average + 0.01 * (delta - delta_average);
	}
};

int time_fps() {
	return (int)(1.0/delta_average);
};


