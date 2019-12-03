#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "stm32f10x.h"
typedef enum{
	L1,
	L2,
	R1,
	R2
}Motor;

typedef enum{
	STOP,
	REVERSE,
	FORWARD
}Dir;
extern u8 ray,ray1,ray2,ray3;
void motor_config(void);
void motor(Motor motor_n,Dir dir,u16 arr);
#endif