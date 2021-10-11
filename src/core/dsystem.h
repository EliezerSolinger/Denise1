#include <stdbool.h>

#ifndef GROTA_WINDOW_H
    #define GROTA_WINDOW_H

    void window_close();
    bool window_init(const char *window_title);
    int window_width();
    int window_height();
    
    bool window_update();
    void window_destroy();

    float window_aspect();

    #define INPUT_PRESSED 1
    #define INPUT_PRESSING 2
    #define INPUT_RELEASED 3

    #define INPUT_MAX_KEYS 20

    int input_status(int key);
    bool input_pressing(int key);
    bool input_pressed(int key);
    bool input_released(int key);

#endif


#ifndef NK_TIME_H
    #define NK_TIME_H

    /// IMPLEMENT TIME FOR BENCHMARKS

    void time_init();
    float time_elapsed();
    float time_elapsed_since(float time_point);

    long time_elapsed_millis();
    long time_elapsed_micro();

    float time_delta();
    float time_average_delta();
    void time_update();
    int time_fps();
#endif
