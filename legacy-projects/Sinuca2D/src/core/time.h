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
