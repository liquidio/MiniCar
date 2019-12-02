#include "ray.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
u16 front_status,back_status;

/**************
 * 4路红外检测初始化
 * PB4、6、8 PD2:输出
 *	PB3、5、7 PC12：捕获
 * 接收到则反光
 * */

void ray_config(void){
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO外设时钟使能                                                        	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/***********
 * 红外捕获
 * */
u8 Ray_cap;
u8 ray,ray1,ray2,ray3;
void  ray_scan(void){
		ray = 1;
	ray1 = 1;
	ray2 =1;
	ray3 =1;
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_11);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	delay_ms(5);
	if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)){
		ray = 0;
	}
	
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	delay_ms(5);
	if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)){
		ray1 = 0;
	}
	
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_11);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	delay_ms(5);
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)){
		ray2 = 0;
	}
	
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOC,GPIO_Pin_11);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	delay_ms(5);
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)){
		ray3 = 0;
	}
}