#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>
#ifndef u8
#define u8 unsigned char
#endif
#ifndef u16
#define u16 unsigned short
#endif
#ifndef u32
#define u32 unsigned int
#endif

#define UART_ON
//#define A_STAR_DEBUG
#define ARR 1000//@TODO:default;
#define PSC 71//@TODO:default;
#define K_deg 1//@TODO:default;
#define K_VELOCITY 10
//control.c
#define SLOW_SPEED 0 //@TODO:DEFAULT,需要填写一个合适的值;
#define MEDIUM_SPEED 50

#endif