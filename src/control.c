#include "control.h"
#include "motor.h"
#include "config.h"
#include "delay.h"
#include "ray.h"
RunStatus Run_status;
extern u8 ray,ray1,ray2,ray3;
extern PStack goal;

static void left(u8);
static void right(u8);
static void back(u8);

/**************
*状态检测RunStatus
*
**/
u8 node = 0,pre_node_status = 0;
void check_track(void){
	if (ray){
		Run_status.in_node = true;
		pre_node_status = 1;
	}else Run_status.in_node = false;
	if (!ray1 && !ray2){
		Run_status.in_track = true;
	}else Run_status.in_track = false;
	
	if (Run_status.in_node == false){
		if (pre_node_status){
			node ++;
			pre_node_status = 0;
		}
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
PStack track;
extern Position current;
Position Agoal;
void run(u8 arr)//@TODO:未使用pid调节，
{	
	Turn next;
	Position Agoal_t;
	if (is_equal_pos(current,Agoal)){
		if (goal.top)
			Agoal = goal.data[goal.top--];
	}
	if (track.top == 0){
		track = a_star(current,Agoal);
	}else {
		Agoal_t = track.data[track.top--];
	}
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
	if (!ray || !ray3){
		pre_node_status = 1;	
	}else if (ray || ray3){
		if (pre_node_status){
			node ++;//经过的节点数
			current.x = Agoal.x;//@TODO:更新CURRENT
			current.y = Agoal.y;
			pre_node_status = 0;
			next = direction_clac(&track,Agoal_t);
			if (next == RIGHT){
				right(arr);
			}
			if (next == LEFT){
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
	delay_ms(TURN_LEFT_K);
}
static void right(u8 arr)
{
		motor(R1,REVERSE,arr);
		motor(L1,FORWARD,arr);
		motor(R2,REVERSE,arr);
		motor(L2,FORWARD,arr);
		delay_ms(TURN_RIGHT_K);
}
static void back(u8 arr)
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