#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "struct_typedef.h"

/* 云台 -> 底盘：目标电机速度 + 目标舵机速度 */
#define CAN_GIMBAL_TO_CHASSIS_ID   0x110U
/* 底盘 -> 云台：实际电机转速 + 在线标志 */
#define CAN_CHASSIS_TO_GIMBAL_ID   0x113U

/* 帧数据长度 */
#define CAN_DATA_LEN               8U

/* 超过该时间未收到对方心跳帧，判定板间通信异常 */
#define BOARD_COMM_TIMEOUT_MS      100U

typedef struct
{
    /* 云台下发 */
    int16_t motor_target_speed;    /* 目标电机速度，单位 rpm */
    int16_t servo_target_speed;    /* 目标舵机速度，-1000~+1000 */

    /* 底盘回传 */
    int16_t motor_actual_speed;    /* 实际电机转速，单位 rpm */
    uint8_t motor_online;          /* 电机在线标志 1=在线 0=异常 */
} comm_data_t;

extern comm_data_t comm;

#endif /* PROTOCOL_H */
