#ifndef GROTA_WINDOW_H
#define GROTA_WINDOW_H
#include <stdbool.h>
#define INPUT_PRESSED 1
#define INPUT_PRESSING 2
#define INPUT_RELEASED 3

#define INPUT_MAX_KEYS 20
void window_close();
bool window_init(const char *window_title,bool antialiasing);
void window_title(const char *window_title);
void* window_pointer();
bool window_update();
void window_destroy();
int window_width();
int window_height();

float window_aspect();
int input_status(int key);
bool input_pressing(int key);
bool input_pressed(int key);
bool input_released(int key);

#endif
