#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "finder.h"
#include "config.h"
#include "stm32f10x.h"
typedef struct{
    volatile bool has_catch;
    volatile bool has_obstacle;
    volatile unsigned char ray_cap;
}CaptureStatus;

typedef struct{
		int set_point;
    int sum_error;
    int pre_error;
    int last_error;
    int proportion;
    int intergral;
    int derivative;
}PID;

typedef enum{
	TURN_LEFT,
	TURN_RIGHT,
	ON_CHG,
	TURN_BACK
	}Turn;

Turn direction_clac(PStack* track,Position current);
void stop(void);
int catch_thing(void);
int place_thing(void);

void run(u8);

#endif