#include "control.h"
#include "motor.h"
#include "config.h"
#include "delay.h"
#include "ray.h"
RunStatus Run_status;
extern u8 Ray_cap;
extern u8 ray,ray1,ray2,ray3;
extern PStack goal;
/**************
*状态检测RunStatus
*
**/
void check_track(void){
	if (Ray_cap == 0x6 ||
		Ray_cap == 0x9 || 
		//Ray_cap == 0xf ||
		Ray_cap == 0xc ||
		Ray_cap == 0x3 ||
		Ray_cap == 0x7 ||
		Ray_cap == 0xe){
			Run_status.in_track = true;
			Run_status.in_node = false;
		}else if (Ray_cap == 0xf){
			Run_status.in_node = true;
	}
	if (Ray_cap == 0){
		Run_status.in_track = false;
		Run_status.in_node = false;
	}
}
void check_finish(Position pos,Position Agoal){
	if (is_equal_pos(pos,Agoal)){
		Run_status.reach_goal++;
	}
	if (is_equal_pos(pos,goal.data[goal.top]) &&
			Run_status.reach_goal == goal.top){
		Run_status.has_finish = true;
	}
}
/***************************
 * 进入赛道
 * */
void into_track(void)
{
    forward(SLOW_SPEED);
    while (!Run_status.in_track){
    }
}
/****************************
 * 方向控制，motor的包装
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
void forward(u8 arr)//@TODO:未使用pid调节，
{	
	ray_scan();
	if(!ray1 && !ray2){
		offset_on(arr);
		
	}
	if (!ray1 && ray2){
		offset_l(arr);
	}
	if (ray1 && !ray2){
		offset_r(arr);
	}
	if (!ray && !ray3){
		right(arr);
	}
}
void left(u8 arr)
{
		motor(L1,REVERSE,arr);
		motor(R1,FORWARD,arr);
		motor(L2,REVERSE,arr);
		motor(R2,FORWARD,arr);
	delay_ms(670);//空车转90,不带抓
}
void right(u8 arr)
{
		motor(R1,REVERSE,arr);
		motor(L1,FORWARD,arr);
		motor(R2,REVERSE,arr);
		motor(L2,FORWARD,arr);
		delay_ms(670);
}
void back(u8 arr)
{
		motor(L1,REVERSE,arr/2);
		motor(R1,REVERSE,arr/2);
		motor(L2,REVERSE,arr/2);
		motor(R2,REVERSE,arr/2);
}
void stop(){
		motor(L1,STOP,0);
		motor(R1,STOP,0);
		motor(L2,STOP,0);
		motor(R2,STOP,0);
}
/**********************
*爪取操作
*@TODO：尚未开始写
**/
int catch_thing(void)
{
	return 0;
}
int place_thing(void)
{
	return 0;
}

//超声波检查
bool check_unknow_position(Position pos){
	if (is_equal_pos(pos,make_pos(2,3))||is_equal_pos(pos,make_pos(3,6))){
	}
	return false;
}

//转向计算
extern PStack goal;
extern Position current;
extern Position pos_nil;
static int goal_index = 0,node_index = 0;
Turn direction_clac(PStack* track,Position current)
{
    int x, y;
	if ( node_index < track->top){
    x = track->data[goal_index].x - current.x;
    y = track->data[goal_index++].y - current.y;
    if (x ^ y)
    {
        if (x >0){
					return LEFT;
				}else if(x <0){
					return RIGHT;
				}
    }
	}
}
/************************************************
 * 运行控制(主控)
 * 
 * **/
void run()
{
    Turn next;
		Position Agoal = goal.data[goal_index];
		PStack track ={0};
		check_finish(current,Agoal);
		if(Run_status.reach_goal == goal_index){
			track.top = 0;
			track = a_star(current,Agoal);
			goal_index ++;
		}else if (Run_status.in_node &&
				!(Run_status.reach_goal == goal_index)){
			next = direction_clac(&track,Agoal);
		}else if (Run_status.has_finish){
			
		}
    if (Run_status.in_node){
				forward(SLOW_SPEED);
				if (next == RIGHT){
					right(MEDIUM_SPEED);
				}
				if (next == LEFT){
					left(MEDIUM_SPEED);
				}
    }else{
        forward(MEDIUM_SPEED);
    }
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