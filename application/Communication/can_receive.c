#include "can_receive.h"
#include "protocol.h"
#include "board_config.h"
#include "can.h"

/* 板间共享数据（由收发共同维护，供控制/显示使用） */
comm_data_t comm;

/* 最近一次收到对方帧的时间戳，用于心跳超时判定 */
static volatile uint32_t board_comm_last_tick = 0;

/**
 * @brief 板间通信是否在线（心跳超时判定）
 * @return 1=在线 0=离线
 */
uint8_t board_comm_is_online(void)
{
    return (uint8_t)((HAL_GetTick() - board_comm_last_tick) < BOARD_COMM_TIMEOUT_MS);
}

typedef union
{
    int16_t value;
    uint8_t data[2];
} int16_bytes_t;

/* 底盘板解析云台下发的控制帧 */
#if BOARD_CHASSIS
static void gimbal_to_chassis_can(uint32_t can_id, const uint8_t *rx_data)
{
    int16_bytes_t u;

    if (can_id != CAN_GIMBAL_TO_CHASSIS_ID)
        return;

    u.data[0] = rx_data[0];
    u.data[1] = rx_data[1];
    comm.motor_target_speed = u.value;

    u.data[0] = rx_data[2];
    u.data[1] = rx_data[3];
    comm.servo_target_speed = u.value;

    board_comm_last_tick = HAL_GetTick();
}
#endif

/* 云台板解析底盘回传的状态帧 */
#if BOARD_GIMBAL
static void chassis_to_gimbal_can(uint32_t can_id, const uint8_t *rx_data)
{
    int16_bytes_t u;

    if (can_id != CAN_CHASSIS_TO_GIMBAL_ID)
        return;

    u.data[0] = rx_data[0];
    u.data[1] = rx_data[1];
    comm.motor_actual_speed = u.value;

    comm.motor_online = rx_data[2];

    board_comm_last_tick = HAL_GetTick();
}
#endif

/**
 * @brief CAN FIFO1 消息挂起回调（在中断上下文执行）
 */
void HAL_CAN_RxFIFO1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data);

#if BOARD_GIMBAL
    chassis_to_gimbal_can(rx_header.StdId, rx_data);
#elif BOARD_CHASSIS
    gimbal_to_chassis_can(rx_header.StdId, rx_data);
#endif
}
