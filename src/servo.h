#ifndef __SERVO_H__
#define __SERVO_H__
#include "stm32f10x.h"
void servo_config(void);

typedef enum {
    R4,
    R3,
    R1,
    R2
}Servo;

void servo(Servo ste,u16 deg);
#endif