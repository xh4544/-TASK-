#include "PID.h"

enum
{
    PID_LAST = 0,
    PID_NOW  = 1,
};

static void abs_limit(fp32 *a, fp32 abs_max)
{
    if (*a > abs_max)  *a = abs_max;
    if (*a < -abs_max) *a = -abs_max;
}

/**
 * @brief PID 初始化
 */
void pid_init(pid_type_def *pid, fp32 max_out, fp32 integral_limit, fp32 kp, fp32 ki, fp32 kd)
{
    pid->max_output     = max_out;
    pid->integral_limit = integral_limit;
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid_reset(pid);
}

/**
 * @brief PID 复位（清零所有内部状态）
 */
void pid_reset(pid_type_def *pid)
{
    pid->set = 0.0f;
    pid->get = 0.0f;
    pid->err[PID_LAST] = 0.0f;
    pid->err[PID_NOW]  = 0.0f;
    pid->sum_err = 0.0f;
    pid->pout = 0.0f;
    pid->iout = 0.0f;
    pid->dout = 0.0f;
    pid->out  = 0.0f;
}

/**
 * @brief 位置式 PID 计算
 * @param pid PID 结构体
 * @param get 反馈值
 * @param set 目标值
 * @return    PID 输出
 */
fp32 pid_calc(pid_type_def *pid, fp32 get, fp32 set)
{
    pid->get = get;
    pid->set = set;
    pid->err[PID_NOW] = set - get;

    pid->pout = pid->p * pid->err[PID_NOW];
    pid->iout += pid->i * pid->err[PID_NOW];
    pid->dout = pid->d * (pid->err[PID_NOW] - pid->err[PID_LAST]);

    abs_limit(&pid->iout, pid->integral_limit);
    pid->out = pid->pout + pid->iout + pid->dout;
    abs_limit(&pid->out, pid->max_output);

    pid->err[PID_LAST] = pid->err[PID_NOW];

    return pid->out;
}
