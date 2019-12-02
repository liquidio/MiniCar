#include "stm32f10x.h"
#include "config.h"
#include "control.h"
#include "led.h"
#include "motor.h"
#include "delay.h"
//#define DEBUG
extern PStack goal;

int main(void){
	led_config();
	motor_config();
	while(1){
		forward(30);
		delay_ms(1000);
		right(100);
	}
}