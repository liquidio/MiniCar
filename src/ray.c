#include "ray.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

u16 front_status,back_status;

/**************
 * 4路红外检测初始化
 * PB5~8:输出
 *	PB0、1、11、12：捕获
 * 接收到则反光
 * */

void ray_config(void){
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设时钟使能                                                        	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/***********
 * 红外捕获
 * */
volatile u8 Ray_cap;
void  ray_scan(void){
		Ray_cap = 0;
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		GPIO_WriteBit(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8,Bit_RESET);
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) &&
			GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5)){
		Ray_cap = 1;
	}
	
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_WriteBit(GPIOB,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8,Bit_RESET);
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) &&
			GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6)){
		Ray_cap |= 1<<1;
	}
	
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		GPIO_WriteBit(GPIOB,GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_8,Bit_RESET);
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) &&
			GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7)){
		Ray_cap |= 1<<2;
	}
	
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
		GPIO_WriteBit(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5,Bit_RESET);;
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) &&
			GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8)){
		Ray_cap |= 1<<3;
	}
}