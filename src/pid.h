#ifndef __PID_H__
#define __PID_H__
typedef struct{
		int set_point;
    int sum_error;
    int pre_error;
    int last_error;
    int proportion;
    int intergral;
    int derivative;
}PID;
unsigned int PIDCalc(PID *pp, unsigned int NextPoint);
#endif