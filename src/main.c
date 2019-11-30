#include "stm32f10x.h"
#include "config.h"
#include "control.h"
#include "led.h"
#define DEBUG
extern PStack goal;
#ifndef DEBUG
#ifndef A_STAR_DEBUG
int main(void){
	int key_cap;
	while(!(key_cap==KEY_Scan(0))){
	}
	if (key_cap ==2){
		map_mirror();
		goal_mirror();
	}
	into_track();
	while(1){
		ray_scan();//红外扫描全局Ray_cap为结果
		check_track();
		#ifdef UART_ON
		#endif
		run();
	}
}
#endif
#endif 
#ifdef DEBUG
int main(void){
	led_config();
	while(1){
		if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)){
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			}
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	}
}
#endif
