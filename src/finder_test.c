#include "config.h"
#ifdef A_STAR_DEBUG
#include "finder.h"
#include <stdio.h>
extern short int map[MAPSIZE_X][MAPSIZE_Y];
extern PStack goal;
int main(){
    Position TestGoal = {6,7};
		Position current = {3,0};
		PStack track;
		track = a_star(current,TestGoal);
		while (track.top){
			printf("x:%d y:%d\n",track.data[track.top].x,track.data[track.top].y);
			track.top--;
		}
}
#endif