#include "control.h"
#include "motor.h"
#include "config.h"
#include "delay.h"
#include "ray.h"
#include "sonar.h"
#include "servo.h"

static void left(u8);
static void right(u8);
static void back(u8);

/**************
*状态检测RunStatus
*
**/
u8 pre_node_status = 0;

/****************************
 * 运行控制
 * */
static void offset_r(u8 arr){
		motor(L1,FORWARD,arr);
		motor(R1,FORWARD,0);
		motor(L2,FORWARD,arr);
		motor(R2,FORWARD,0);
}
static void offset_l(u8 arr){
		motor(R1,FORWARD,arr);
		motor(L1,FORWARD,0);
		motor(R2,FORWARD,arr);
		motor(L2,FORWARD,0);
}
static void offset_on(u8 arr){
		motor(L1,FORWARD,arr);
		motor(R1,FORWARD,arr);
		motor(L2,FORWARD,arr);
		motor(R2,FORWARD,arr);
}

PStack track;

u8 map_new = 0;
u16 direction = 90;//记录车身方向

extern u8 ray,ray1,ray2,ray3;//红外扫描结果
extern PStack goal;
extern Position current;

static Position Agoal;
static float distance = 0.0;
static Turn next;
static	Position Agoal_t;//下一地点
extern short int map[MAPSIZE_X][MAPSIZE_Y];
void run(u8 arr)//@TODO:未使用pid调节，
{	

}
static void left(u8 arr)
{
		motor(L1,REVERSE,arr);
		motor(R1,FORWARD,arr);
		motor(L2,REVERSE,arr);
		motor(R2,FORWARD,arr);
		delay_ms(TURN_LEFT_K);//简单的转弯
		
		//更新车身方向
		if (direction == 90 ) direction = 180;
		if (direction == 0 ) direction = 90;
		if (direction == 180) direction = 270;
		if (direction == 270) direction = 0;
	
		if (distance < 1e-1){
				map[track.data[track.top--].x][track.data[track.top--].y] = -1;
				map_new = 1;
		}
}
static void right(u8 arr)
{
		motor(R1,REVERSE,arr);
		motor(L1,FORWARD,arr);
		motor(R2,REVERSE,arr);
		motor(L2,FORWARD,arr);
		delay_ms(TURN_RIGHT_K);
		if (direction == 90 ) direction = 0;
		if (direction == 0 ) direction = 270;
		if (direction == 180) direction = 90;
		if (direction == 270) direction = 180;
	
		if (distance < 1e-1){
				map[track.data[track.top--].x][track.data[track.top--].y] = -1;
				map_new = 1;
		}
}
static void back(u8 arr)
{	
	if (!ray1 && !ray2){
		motor(L1,REVERSE,arr);
		motor(R1,REVERSE,arr);
		motor(L2,REVERSE,arr);
		motor(R2,REVERSE,arr);
	}
	if (!ray1 && ray2){
		offset_r(arr);
	}
	if (ray1 && !ray2){
		offset_l(arr);
	}
}
void stop(){
		motor(L1,STOP,0);
		motor(R1,STOP,0);
		motor(L2,STOP,0);
		motor(R2,STOP,0);
}
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
static void hand_left(void){
	servo(S1,0.015*(2e4-1));//0.015的占空比
	delay_ms(500);
	hand_deg --;
}
static void hand_right(void){
	servo(S1,0.025*(2e4-1));//0.025的占空比
	delay_ms(500);
	hand_deg ++;
}

//车身转向计算
extern PStack goal;
extern Position current;
extern Position pos_nil;

/*****************************************************************
 * pid计算函数
 * */
unsigned int PIDCalc(PID *pp, unsigned int NextPoint)
{
    unsigned int d_error, error;
    error = pp->set_point - NextPoint;
    pp->sum_error += error;
    d_error = pp->last_error - pp->pre_error;
    pp->pre_error = pp->last_error;
    pp->last_error = error;
    return pp->proportion * error +
           pp->intergral * pp->sum_error +
           pp->derivative * d_error;
}