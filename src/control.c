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
	if (is_equal_pos(current,Agoal)){//从目标栈里获取目标地点
		if (goal.top)
			Agoal = goal.data[goal.top--];
	}
	if (track.top == 0 || map_new){//加载到达目的路径
		track = a_star(current,Agoal);
		if (map_new) map_new = 0;
	}else {
		Agoal_t = track.data[track.top--];//下一地点
	}
	
	ray_scan();//红外扫描(20ms)
	distance = sonar_scan_barrier();//超声波扫描;最长10ms
	
	if (distance < 2e-1){
		//将障碍物位置记录到地图并，并重新计算路径
		map[track.data[track.top--].x][track.data[track.top--].y] = -1;
		map_new = 1;
		return;
	}
	
	//初始
	if(!ray1 && !ray2){
		offset_on(arr);
		
	}
	if (!ray1 && ray2){
		offset_l(arr);
	}
	if (ray1 && !ray2){
		offset_r(arr);
	}
	if (!ray || !ray3){
		pre_node_status = 1;
	}else if (ray || ray3){
		if (pre_node_status){
			current.x = Agoal.x;//更新CURRENT
			current.y = Agoal.y;
			pre_node_status = 0;
			next = direction_clac(&track,Agoal_t);//查取转向
			if (next == TURN_RIGHT){//右转90deg
				right(arr);
			}else
			if (next == TURN_LEFT){//左转90deg
				left(arr);
				}else
			if (next == TURN_BACK){
				left(arr);
				left(arr);
			}
			}
	}
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
static int goal_index = 0,node_index = 0;

Turn direction_clac(PStack* track,Position current)
{
  short int x, y;
	if ( node_index < track->top){
    x = track->data[goal_index].x - current.x;
    y = track->data[goal_index++].y - current.y;
    if (x ^ y)//转向对照表
    {
        if (x >0){
					//next direction = 90;
					if(direction == 0) return TURN_LEFT;
					if(direction == 90) return ON_CHG;
					if(direction == 180) return TURN_RIGHT;
					if(direction == 270) return TURN_BACK;
				}else if(x <0){
					//next direction = 270;
					if(direction == 0) return TURN_RIGHT;
					if(direction == 90) return TURN_BACK;
					if(direction == 180) return TURN_LEFT;
					if(direction == 270) return ON_CHG;
				}
				if (y>0){
					//next direction = 0;
					if(direction == 0) return ON_CHG;
					if(direction == 90) return TURN_LEFT;
					if(direction == 180) return TURN_BACK;
					if(direction == 270) return TURN_RIGHT;
				}else if (y<0){
					//next direction = 180;
					if(direction == 0) return TURN_BACK;
					if(direction == 90) return TURN_RIGHT;
					if(direction == 180) return ON_CHG;
					if(direction == 270) return TURN_LEFT;
				}
    }
	}
	return ON_CHG;
}
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