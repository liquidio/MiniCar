#ifndef __SERVO_H__
#define __SERVO_H__
#include "stm32f10x.h"
void servo_config(void);

typedef enum {
	S1,
	S2,
	S3,
	S4,
}Servo;
//deg(0.015*2e4,0.025*2e4)
void servo(Servo ste,u16 deg);
#endif