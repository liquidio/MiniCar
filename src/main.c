#include "stm32f10x.h"
#include "config.h"
#include "control.h"
#include "led.h"
#include "motor.h"
#include "delay.h"
#include "ray.h"
#include "sonar.h"

//#define DEBUG
extern PStack goal;

int main(void){
	led_config();
	motor_config();
	ray_config();
	sonar_config();
	while(1){
		run(75);
	}
}