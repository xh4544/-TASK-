#include "gimbal.h"
#include "bsp_joystick.h"
#include "bsp_servo.h"
#include "bsp_led.h"
#include "bsp_can.h"
#include "can_send.h"
#include "can_receive.h"
#include "protocol.h"
#include "can.h"
#include <stdio.h>

/* 摇杆满量程对应的电机目标转速 rpm */
#define JOY_TO_MOTOR_RPM   300

void gimbal_init(void)
{
    bsp_joystick_init();
    bsp_servo_init();

    comm.motor_target_speed = 0;
    comm.servo_target_speed = 0;
}

/**
 * @brief 云台板任务
 *   摇杆 X 轴 -> 舵机
 *   摇杆 Y 轴 -> 电机目标转速，经 CAN 下发底盘
 *   CAN 通信异常 -> 呼吸灯
 */
void gimbal_task(void)
{
    int16_t jx, jy;

    /* CAN错误恢复：总线关闭时自动复位，防止卡死 */
    bsp_can_error_recover();

    bsp_joystick_read(&jx, &jy);

    /* 舵机控制 */
    comm.servo_target_speed = jx;
    bsp_servo_set_speed(jx);

    /* 电机目标转速经CAN下发 */
    comm.motor_target_speed = (int16_t)((int32_t)jy * JOY_TO_MOTOR_RPM / 1000);

    gimbal_send_control(comm.motor_target_speed, comm.servo_target_speed);

    /* 诊断：串口打印 CAN 状态（每 500ms 一次）
    static uint32_t last_print = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_print >= 500)
    {
        last_print = now;
        printf("CAN init_state=%lu cfg=%lu start=%lu tx_ok=%lu tx_fail=%lu mbox=%lu ESR=0x%lx TSR=0x%lx\r\n",
               (unsigned long)can_init_state, (unsigned long)can_cfg_status,
               (unsigned long)can_start_status,
               (unsigned long)can_tx_ok, (unsigned long)can_tx_fail,
               (unsigned long)can_mailbox_free(),
               (unsigned long)hcan.Instance->ESR,
               (unsigned long)hcan.Instance->TSR);
    } */

    /* 底盘离线 -> 呼吸灯 */
    if (board_comm_is_online())
        bsp_led_off();
    else
        bsp_led_breathing_tick();
}
