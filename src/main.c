#include "stm32f10x.h"

#include "config.h"
#include "led.h"
#include "delay.h"
#include "ray.h"
#include "motor.h"
#include "servo.h"
#include "sonar.h"

#include "turn.h"
#include "hand.h"
#include "finder.h"

//全局参数
Position Agoal,Agoal_t;
u8 map_new = 0,pre_node_status = 0;
PStack track;
float distance = 0.0;

int main(void){
	u16 arr = 75;
	Turn next;
	led_config();
	motor_config();
	ray_config();
	sonar_config();
	servo_config();
	while(1){
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
				continue;
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
}