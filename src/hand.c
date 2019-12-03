#include "hand.h"
#include "servo.h"
#include "delay.h"

/**********************
*爪取
*收爪
*@TODO：参数未调
**/
int catch_thing(void)
{
	servo(S2,0.016*(2e4-1));
	servo(S3,0.016*(2e4-1));
	servo(S4,0.016*(2e4-1));
	return 0;
}
int place_thing(void)
{
	servo(S2,0.021*(2e4-1));
	servo(S3,0.021*(2e4-1));
	servo(S4,0.021*(2e4-1));
	return 0;
}
static u8 hand_deg = 0;

//机械爪旋转
void hand_left(void){
	servo(S1,0.015*(2e4-1));//0.015的占空比
	delay_ms(500);
	hand_deg --;
}
void hand_right(void){
	servo(S1,0.025*(2e4-1));//0.025的占空比
	delay_ms(500);
	hand_deg ++;
}
