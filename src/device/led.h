#ifndef __LED_H__
#define __LED_H__
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
/******
 * 位绑定
 * */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	//错误提示灯

void led_config(void);

#endif