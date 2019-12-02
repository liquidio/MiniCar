#include "stm32f10x.h"
#include "config.h"
#include "control.h"
#include "led.h"
#include "motor.h"
#include "delay.h"
#include "ray.h"
//#define DEBUG
extern PStack goal;

int main(void){
	u8 arr =30;
	led_config();
	motor_config();
	ray_config();
	while(1){
		forward(75);
//		motor(L1,FORWARD,arr);
//		motor(R1,FORWARD,arr);
//		motor(L2,FORWARD,arr*2);
//		motor(R2,FORWARD,arr);
	}
}