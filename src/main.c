#include "stm32f10x.h"
#include "config.h"
#include "control.h"
#include "led.h"
//#define DEBUG
extern PStack goal;

int main(void){
	led_config();
	while(1){
		forward(1000);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	}
}