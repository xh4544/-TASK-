#include "oled_app.h"
#include "bsp_oled.h"
#include "protocol.h"
#include "can_receive.h"
#include "bsp_can.h"
#include "tim.h"
#include "can.h"

/*
 * OLED 显示布局（6x8 字库，8 页，21 列）
 *  Line0: 舵机目标转速 + 在线(云台在线)
 *  Line1: 电机目标转速
 *  Line2: 电机实际转速 + 在线
 *  Line3: 板间通信状态
 */
void oled_app_init(void)
{
    bsp_oled_init();
}

void oled_app_task(void)
{
    uint8_t board_on = board_comm_is_online();

    bsp_oled_clear();

    /* 舵机：目标转速 + 在线状态（舵机在云台端，在线=云台在线） */
    bsp_oled_show_str(0, 0, "Srv:");
    bsp_oled_show_num(30, 0, comm.servo_target_speed, 4);
    bsp_oled_show_str(60, 0, board_on ? "ON" : "OFF");

    /* 电机：目标转速 */
    bsp_oled_show_str(0, 1, "MtrT:");
    bsp_oled_show_num(30, 1, comm.motor_target_speed, 4);

    /* 电机：实际转速 + 在线 */
    bsp_oled_show_str(0, 2, "MtrA:");
    bsp_oled_show_num(30, 2, comm.motor_actual_speed, 4);
    bsp_oled_show_str(60, 2, comm.motor_online ? "ON" : "OFF");

    /* 板间通信状态 */
    bsp_oled_show_str(0, 3, "Comm:");
    bsp_oled_show_str(30, 3, board_on ? "OK" : "LOST");

    /* 调试：CAN 收发计数（发送成功/失败、接收计数、bus-off 次数） */
    bsp_oled_show_str(0, 4, "TX:");
    bsp_oled_show_num(24, 4, (int32_t)can_tx_ok, 6);
    bsp_oled_show_num(66, 4, (int32_t)can_tx_fail, 5);
    bsp_oled_show_str(0, 5, "RX:");
    bsp_oled_show_num(24, 5, (int32_t)can_rx_count, 6);
    bsp_oled_show_str(0, 6, "BOFF:");
    bsp_oled_show_num(30, 6, (int32_t)can_bus_off_count, 4);
    /* 调试：FIFO0 / FIFO1 待处理帧数，看帧进哪个 FIFO */
    bsp_oled_show_str(0, 7, "F:");
    bsp_oled_show_num(18, 7, (int32_t)(hcan.Instance->RF0R & CAN_RF0R_FMP0), 1);
    bsp_oled_show_num(36, 7, (int32_t)(hcan.Instance->RF1R & CAN_RF1R_FMP1), 1);

    bsp_oled_flush();
}
