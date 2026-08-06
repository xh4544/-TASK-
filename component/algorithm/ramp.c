#include "ramp.h"

/**
 * @brief 斜坡初始化
 * @param slope 每个调用周期允许的最大变化量
 * @param max   输出上限
 * @param min   输出下限
 */
void ramp_init(ramp_function_source_t *ramp, fp32 slope, fp32 max, fp32 min)
{
    ramp->slope     = slope;
    ramp->max_value = max;
    ramp->min_value = min;
    ramp->input     = 0.0f;
    ramp->out       = 0.0f;
}

/**
 * @brief 斜坡计算：out 以 slope 为限逐步逼近 input
 */
void ramp_calc(ramp_function_source_t *ramp, fp32 input)
{
    ramp->input = input;

    fp32 delta = input - ramp->out;

    if (delta > ramp->slope)
        ramp->out += ramp->slope;
    else if (delta < -ramp->slope)
        ramp->out -= ramp->slope;
    else
        ramp->out = input;

    if (ramp->out > ramp->max_value)
        ramp->out = ramp->max_value;
    else if (ramp->out < ramp->min_value)
        ramp->out = ramp->min_value;
}
