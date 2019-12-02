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

unsigned int PIDCalc(PID *pp, unsigned int NextPoint);
void into_track(void);
void forward(u8 speed);
void left(u8 speed);
void right(u8 speed);
void back(u8 speed);
int catch_thing(void);
int place_thing(void);

void run(void);

void check_finish(Position pos,Position Agoal);
void check_track(void);

#endif