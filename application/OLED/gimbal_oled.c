#include "gimbal_oled.h"
#include "bsp_oled_i2c.h"
#include "bsp_can.h"
#include "can_receive.h"
#include "protocol.h"

/*
 * 云台板 OLED（I2C2）显示内容：
 *  Line0: 发送成功 / 失败计数
 *  Line1: 接收计数
 *  Line2: 板间通信状态
 *  Line3: 底盘回传的电机实际转速 + 在线
 */
void gimbal_oled_init(void)
{
    bsp_oled_i2c_init();
}

void gimbal_oled_task(void)
{
    bsp_oled_i2c_clear();

    /* 发送计数：成功 / 失败 */
    bsp_oled_i2c_show_str(0, 0, "TX:");
    bsp_oled_i2c_show_num(24, 0, (int32_t)can_tx_ok, 6);
    bsp_oled_i2c_show_str(0, 1, "TF:");
    bsp_oled_i2c_show_num(24, 1, (int32_t)can_tx_fail, 6);

    /* 接收计数 */
    bsp_oled_i2c_show_str(0, 2, "RX:");
    bsp_oled_i2c_show_num(24, 2, (int32_t)can_rx_count, 6);

    /* 板间通信状态 */
    bsp_oled_i2c_show_str(0, 3, "Comm:");
    bsp_oled_i2c_show_str(30, 3, board_comm_is_online() ? "ON " : "OFF");

    /* 底盘回传：电机实际转速 + 在线 */
    bsp_oled_i2c_show_str(0, 4, "MtrA:");
    bsp_oled_i2c_show_num(30, 4, (int32_t)comm.motor_actual_speed, 4);
    bsp_oled_i2c_show_str(60, 4, comm.motor_online ? "ON" : "OFF");

    bsp_oled_i2c_flush();
}
