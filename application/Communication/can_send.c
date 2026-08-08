#include "can_send.h"
#include "protocol.h"
#include "bsp_can.h"

/* 多字节数值与字节流的转换 */
typedef union
{
    int16_t value;
    uint8_t data[2];
} int16_bytes_t;

/**
 * @brief 云台 -> 底盘：下发目标速度
 * 帧 0x110: [0:1] 目标电机速度(int16)  [2:3] 目标舵机速度(int16)
 */
void gimbal_send_control(int16_t motor_target, int16_t servo_target)
{
    uint8_t data[8] = {0};
    int16_bytes_t u;

    u.value = motor_target;
    data[0] = u.data[0];
    data[1] = u.data[1];

    u.value = servo_target;
    data[2] = u.data[0];
    data[3] = u.data[1];

    bsp_can_send(CAN_GIMBAL_TO_CHASSIS_ID, data, 8);
}

/**
 * @brief 回传状态
 * 帧 0x113: [0:1] 实际电机转速(int16)  [2] 电机在线标志
 */
void chassis_send_status(int16_t motor_actual, uint8_t motor_online)
{
    uint8_t data[8] = {0};
    int16_bytes_t u;

    u.value = motor_actual;
    data[0] = u.data[0];
    data[1] = u.data[1];

    data[2] = motor_online;

    bsp_can_send(CAN_CHASSIS_TO_GIMBAL_ID, data, 8);
}
