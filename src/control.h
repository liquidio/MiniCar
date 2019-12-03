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
void run(u8);

#endif