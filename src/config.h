#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>

#define UART_ON
//#define A_STAR_DEBUG
#define TIM_ARR 100//@TODO:default;
#define TIM_PSC 71//@TODO:default;

//control.c
#define SLOW_SPEED 30 //@TODO:DEFAULT,需要填写一个合适的值;
#define MEDIUM_SPEED 50
#define HIGH_SPEED 80

#define TURN_RIGHT_K 670//空车转90,不带抓670
#define TURN_LEFT_K 670

#endif