#ifndef RAMP_H
#define RAMP_H

#include "struct_typedef.h"

/**
 * @brief 斜坡函数：使目标值按固定速率平滑变化，避免阶跃
 */
typedef struct
{
    fp32 input;         /* 输入（期望值） */
    fp32 out;           /* 输出（平滑后的值） */
    fp32 max_value;     /* 限幅最大值 */
    fp32 min_value;     /* 限幅最小值 */
    fp32 slope;         /* 每周期允许的最大变化量 */
} ramp_function_source_t;

void ramp_init(ramp_function_source_t *ramp, fp32 slope, fp32 max, fp32 min);
void ramp_calc(ramp_function_source_t *ramp, fp32 input);

#endif /* RAMP_H */
