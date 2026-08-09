#include "oled_app.h"
#include "bsp_oled.h"
#include "protocol.h"
#include "can_receive.h"

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

    bsp_oled_flush();
}
