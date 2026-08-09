#include "chassis.h"
#include "bsp_motor.h"
#include "bsp_led.h"
#include "can_send.h"
#include "can_receive.h"
#include "protocol.h"
#include "PID.h"
#include "ramp.h"
#include "filter.h"
#include <stdlib.h>

/* ------------------------- 参数（需按实际调参） ------------------------- */
#define MOTOR_MAX_RPM       300     /* 目标转速上限 rpm */
#define PID_MAX_OUTPUT      1000    /* PID 输出上限，对应 duty -1000~1000 */
#define PID_INTEGRAL_LIMIT  500     /* 积分限幅 */
#define SPEED_PID_KP        3.0f    /* 速度环比例 */
#define SPEED_PID_KI        0.1f    /* 速度环积分 */
#define SPEED_PID_KD        0.0f    /* 速度环微分 */

#define RAMP_SLOPE          20.0f   /* 每周期目标转速最大变化量 rpm */
#define SPEED_FILTER_ALPHA  0.3f    /* 测速一阶低通系数 */

/* 堵转检测：目标转速较高但实际转速接近 0 持续一段时间判为异常 */
#define STALL_TARGET_THRESH 100     /* rpm */
#define STALL_SPEED_THRESH  20      /* rpm */
#define STALL_TIMEOUT_CNT   100     /* 周期数（5ms/周期 -> 500ms） */

static pid_type_def          speed_pid;
static ramp_function_source_t motor_ramp;
static first_order_filter_t  speed_filter;
static uint32_t stall_count = 0;

void chassis_init(void)
{
    bsp_motor_init();

    pid_init(&speed_pid, PID_MAX_OUTPUT, PID_INTEGRAL_LIMIT,
             SPEED_PID_KP, SPEED_PID_KI, SPEED_PID_KD);
    ramp_init(&motor_ramp, RAMP_SLOPE, MOTOR_MAX_RPM, -MOTOR_MAX_RPM);
    first_order_filter_init(&speed_filter, SPEED_FILTER_ALPHA);

    comm.motor_actual_speed = 0;
    comm.motor_online = 1;
}

/**
 * @brief 底盘板周期任务（速度闭环）
 */
void chassis_task(void)
{
    int16_t actual;
    int16_t target;
    int16_t duty;
    fp32    filtered;

    /* 1. 测速 + 低通滤波 */
    actual   = bsp_motor_read_speed_rpm();
    filtered = first_order_filter_cali(&speed_filter, (fp32)actual);
    comm.motor_actual_speed = (int16_t)filtered;

    /* 2. 目标转速经斜坡平滑，避免阶跃 */
    ramp_calc(&motor_ramp, (fp32)comm.motor_target_speed);
    target = (int16_t)motor_ramp.out;

    /* 3. 电机堵转/异常检测 */
    if (abs(target) > STALL_TARGET_THRESH && abs((int16_t)filtered) < STALL_SPEED_THRESH)
    {
        if (++stall_count > STALL_TIMEOUT_CNT)
            comm.motor_online = 0;
    }
    else
    {
        stall_count = 0;
        comm.motor_online = 1;
    }

    /* 4. 速度环闭环 */
    if (board_comm_is_online() && comm.motor_online)
    {
        duty = (int16_t)pid_calc(&speed_pid, filtered, (fp32)target);
        bsp_motor_set_duty(duty);
    }
    else
    {
        /* 板间离线或电机异常 -> 停机并复位 PID */
        bsp_motor_set_duty(0);
        pid_reset(&speed_pid);
    }

    /* 5. 回传状态帧（兼作心跳） */
    chassis_send_status((int16_t)filtered, comm.motor_online);

    /* 6. 电机异常 -> 呼吸灯 */
    if (comm.motor_online)
        bsp_led_off();
    else
        bsp_led_breathing_tick();
}
