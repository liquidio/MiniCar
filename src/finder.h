#ifndef __FINDER_H__
#define __FINDER_H__
#include <stdbool.h>

#ifndef NULL
#define NULL 0
#endif 

//搬运场地地图
#define MAPSIZE_X 9
#define MAPSIZE_Y 11

//position pair
typedef struct{
    int x;
    int y;
}Position;

//sequence stack
#define STACK_SIZE 20
typedef struct{
    Position data[STACK_SIZE];
    int top;
}PStack;

//position 用于计算距离的位置数据集
/*struct PositionCehck extend Position
*/
typedef struct POSITIONCHECK{
    Position *position;//current node
    int f_score;
	int g_score;
	int h_score;
}PositionCheck;
extern PStack *run_set;
PStack a_star(Position start,Position goal);
//构建一个Position
Position make_pos(int x, int y);
//初始化位置
void append(PStack *seq,Position data);
Position pop(PStack*);
void set_open_set(PStack* open_set,PStack* close_set,Position current);
Position make_pos(int x,int y);
bool is_equal_pos(Position,Position);
bool has_pos(PStack*,Position*);
bool has_map_obstacle(Position checked);
void goal_mirror(void);
void map_mirror(void);
#endif