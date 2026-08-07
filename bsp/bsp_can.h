#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "main.h"

void bsp_can_init(void);
void bsp_can_send(uint32_t std_id, uint8_t *data, uint8_t len);
void bsp_can_error_recover(void);
uint32_t can_mailbox_free(void);

/* 调试计数器 */
extern volatile uint32_t can_tx_ok;          /* 发送成功次数 */
extern volatile uint32_t can_tx_fail;        /* 发送失败次数 */
extern volatile uint32_t can_rx_count;       /* 接收帧次数 */
extern volatile uint32_t can_bus_off_count;  /* bus-off 复位次数 */

/* 诊断：CAN 初始化状态 */
extern volatile uint32_t can_init_state;     /* hcan.State: 0=RESET 1=READY 2=LISTENING */
extern volatile uint32_t can_cfg_status;     /* HAL_CAN_ConfigFilter 返回值 */
extern volatile uint32_t can_start_status;   /* HAL_CAN_Start 返回值 */

#endif /* BSP_CAN_H */
