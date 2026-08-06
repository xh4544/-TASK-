#ifndef PID_H
#define PID_H

#include "struct_typedef.h"

/**
 * @brief 位置式 PID 控制器（带积分限幅与输出限幅）
 */
typedef struct
{
    fp32 p;                 /* 比例系数 */
    fp32 i;                 /* 积分系数 */
    fp32 d;                 /* 微分系数 */

    fp32 set;               /* 目标值 */
    fp32 get;               /* 反馈值 */
    fp32 err[2];            /* [LAST, NOW] 误差 */
    fp32 sum_err;           /* 积分累计 */

    fp32 pout;
    fp32 iout;
    fp32 dout;

    fp32 out;               /* PID 总输出 */

    fp32 max_output;        /* 输出限幅 */
    fp32 integral_limit;    /* 积分限幅 */
} pid_type_def;

void  pid_init(pid_type_def *pid, fp32 max_out, fp32 integral_limit, fp32 kp, fp32 ki, fp32 kd);
void  pid_reset(pid_type_def *pid);
fp32  pid_calc(pid_type_def *pid, fp32 get, fp32 set);

#endif /* PID_H */
