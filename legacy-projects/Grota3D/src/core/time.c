#include <stdlib.h>
#include <sys/time.h>

long initial_time=0;
long last_update_time=0;
float delta=0;

float delta_average=0;
/**
 * Returns the current time in microseconds using posix function,
 */

static long getSysMicrotime(){
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

