#include <math.h>
#include "finder.h"
#include "config.h"
///////////////////////////初始化
#define GOAL_NUM 7

int error;
short int goal_flag = GOAL_NUM;
PStack* goal_set;
Position start = { 1, 0 };           //给出固定的开始位置
Position current = { 1, 0 };         //初始化当前位置
const Position pos_nil = { -1, -1 }; //位置不存在标识

//init map;
short int map[MAPSIZE_X][MAPSIZE_Y] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,0,0,0},
	{-1,-1,-1,0,1},
	{-1,0,0,0,-1},
	{0,0,-1,0,1},
	{-1,-1,-1,0,-1,1,1,1,-1,0,0},
	{-1,-1,-1,0,-1,0,0,0,-1},
	{-1,-1,-1,0,-1,0,0,0,-1},
	{-1,-1,-1,0,-1,-1,-1,-1,-1},
	{-1,-1,-1,0,-1,-1,-1,-1,-1},
};

PStack goal = { {{0,0},{1, 5}, {6, 5}, {1, 6}, {6, 6}, {1, 7}, {6, 7}, {1, 9}},7 };

//第二个地图为默认地图的x镜像
void map_mirror(void) {
	int i, j;
	short int temp;
	for (i = 0; i <= 4; i++) {
		for (j = 0; j < 9; j++) {
			temp = map[i][j];
			map[i][j] = map[8 - i][j];
			map[8 - i][j] = temp;
		}
	}
}
void goal_mirror(void) {
	int i;
	for (i = 0; i < GOAL_NUM; i++) {
		goal.data[i].x = 9 - goal.data[i].x;
	}
}
//scan open_set获取当前位置的四个方向的有效值
void set_open_set(PStack* open_set,PStack* close_set,Position start)
{
	if (start.x == pos_nil.x &&
		start.y == pos_nil.y) return;
	Position pos;
	int i;
	int dir[][2] = { {-1, 0},
					{0, -1},
					{0, 1},
					{1, 0} };
	for (i = 0; i < 4; i++)
	{
		pos.x = start.x + dir[i][0];
		pos.y = start.y + dir[i][1];
		//越界检测
		if ((pos.x + dir[i][0]) < 0 || (pos.x + dir[i][0]) > 9)
			continue;
		if ((pos.y + dir[i][1]) < 0 || (pos.y + dir[i][1]) > 9)
			continue;
		if (has_pos(close_set, &pos) ||
			has_map_obstacle(pos)) {
			continue;
		}
		if (has_pos(open_set, &pos)) {
			continue;
		}
		append(open_set, pos);
	}
}

void append(PStack* seq, Position data)
{
	if (seq->top < STACK_SIZE - 1) {
		seq->top++;
		seq->data[seq->top].x = data.x;
		seq->data[seq->top].y = data.y;
	}
	else {
		error++;
	}
}
Position make_pos(int x, int y)
{
	Position p = { x, y };
	return p;
}

bool is_equal_pos(Position pos1, Position pos2)
{
	if (pos1.x == pos2.x &&
		pos1.y == pos2.y)
		return true;
	return false;
}
bool has_pos(PStack* pos_set, Position* checked)
{
	int k;
	for (k = 0; k <= pos_set->top; k++)
	{
		if (is_equal_pos(*checked, pos_set->data[k]))
			return true;
	}
	return false;
}
bool has_map_obstacle(Position checked)
{
	if (map[checked.x][checked.y] == -1)
		return true;
	return false;
}
#ifndef A_STAR_DEBUG //@A_STAR_DEBUG:测试A*算法时，可设定没有位置区域。
extern int run(PositionCheck*); //@function:小车运行到目标点
//获取未知地点是否可通行，0表示可通行，-1表示有不可通行，1表示未知;
#endif

void PosCheckInit(PositionCheck* pos) {
	pos->f_score = 0;
	pos->g_score = 0;
	pos->h_score = 0;
	pos->position = NULL;
}
void calcFScore(PositionCheck* target, Position Agoal) {
	//double xm = 0.0;
	double  ym = 0.0;
	//target->g_score++;
	//xm = pow((double)Agoal.x - target->position->x, 2);
	ym = pow((double)Agoal.y - target->position->y, 2);
	target->h_score = ym;
	target->f_score = target->g_score + target->h_score;
}
/************************************
*输入开始当前（开始）坐标和目标坐标，返回目标坐标的一个指针双向链表
*此函数为核心函数，用的是A*算法，H为曼哈顿距离。
*/
PStack a_star(Position start, Position Agoal)
{
	PStack open_set = { 0 }, close_set = { 0 };
	PStack track_set = { 0 };
	PositionCheck neighbor, min_distance;

	while (true)
	{
		set_open_set(&open_set,&close_set,start);//检查周围四个节点,设置open_set
		PosCheckInit(&neighbor);
		Position temp = open_set.data[open_set.top];
		neighbor.position = &temp;
		min_distance = neighbor;
		calcFScore(&min_distance, Agoal);//初始化最小f_score的点
		if (open_set.top == 0) {//只能找到这些了，到不了终点
			if (close_set.top && track_set.top){
				start = close_set.data[close_set.top--];
				append(&close_set,track_set.data[track_set.top--]);
				continue;
			}else{
				return track_set;
			}
		}
		while (open_set.top) {//找到open_set中有最小距离的位置
			neighbor.position = &open_set.data[open_set.top--];
			if (is_equal_pos(*neighbor.position, Agoal))//找到目的地
			{
				append(&track_set, *neighbor.position);
				return track_set;
			}
			if (has_pos(&close_set, neighbor.position))//close_set里是否有neighbor
				continue;
			calcFScore(&neighbor, Agoal);//计算neighbor的f_score
			if (neighbor.f_score < min_distance.f_score)
			{
				min_distance.f_score = neighbor.f_score;
				min_distance.position = neighbor.position;
			}
		}
		append(&track_set, *min_distance.position);//找到最小f_score，开始设置
		append(&close_set, *min_distance.position);
		start = *min_distance.position;
	}
}