#include "turn.h"
#include "motor.h"
#include "delay.h"
#include "config.h"

u16 direction = 90;
extern float distance;
extern PStack track;
extern u8 map_new;

void offset_r(u8 arr){
		motor(L1,FORWARD,arr);
		motor(R1,FORWARD,0);
		motor(L2,FORWARD,arr);
		motor(R2,FORWARD,0);
}
void offset_l(u8 arr){
		motor(R1,FORWARD,arr);
		motor(L1,FORWARD,0);
		motor(R2,FORWARD,arr);
		motor(L2,FORWARD,0);
}
void offset_on(u8 arr){
		motor(L1,FORWARD,arr);
		motor(R1,FORWARD,arr);
		motor(L2,FORWARD,arr);
		motor(R2,FORWARD,arr);
}

void left(u8 arr)
{
		motor(L1,REVERSE,arr);
		motor(R1,FORWARD,arr);
		motor(L2,REVERSE,arr);
		motor(R2,FORWARD,arr);
		delay_ms(TURN_LEFT_K);//?????
		
		//??????
		if (direction == 90 ) direction = 180;
		if (direction == 0 ) direction = 90;
		if (direction == 180) direction = 270;
		if (direction == 270) direction = 0;
	
		if (distance < 1e-1){
				map[track.data[track.top--].x][track.data[track.top--].y] = -1;
				map_new = 1;
		}
}
void right(u8 arr)
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
void back(u8 arr)
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
void stop(void){
		motor(L1,STOP,0);
		motor(R1,STOP,0);
		motor(L2,STOP,0);
		motor(R2,STOP,0);
}
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