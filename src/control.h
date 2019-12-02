#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "finder.h"
#include "config.h"
#include "stm32f10x.h"
typedef struct{
    volatile bool in_track;
    volatile bool in_node;
    volatile bool has_finish;
    volatile int reach_goal;
}RunStatus;

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
	LEFT,
	RIGHT,
	}Turn;

Turn direction_clac(PStack* track,Position current);
void into_track(void);
void stop(void);
int catch_thing(void);
int place_thing(void);

void run(u8);

void check_finish(Position pos,Position Agoal);
void check_track(void);

#endif