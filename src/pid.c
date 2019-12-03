#include "pid.h"

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