#ifndef __TURN_H__
#define __TURN_H__
#include "stm32f10x.h"
#include "finder.h"
typedef enum{
	TURN_LEFT,
	TURN_RIGHT,
	ON_CHG,
	TURN_BACK
	}Turn;

extern u16 direction;
	
void offset_r(u8 arr);
void offset_l(u8 arr);
void offset_on(u8 arr);
void left(u8 arr);
void right(u8 arr);
void back(u8 arr);
void stop(void);
Turn direction_clac(PStack* track,Position current);

#endif