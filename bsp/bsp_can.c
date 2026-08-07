#include "bsp_can.h"
#include "can.h"
#include <stdio.h>

/* 调试计数器 */
volatile uint32_t can_tx_ok = 0;
volatile uint32_t can_tx_fail = 0;
volatile uint32_t can_rx_count = 0;
volatile uint32_t can_bus_off_count = 0;

/**
 * @brief 初始化 CAN 滤波器并启动接收
 *   1. 配置滤波器：接收 0x110 ~ 0x11F 区间的标准帧，存入 FIFO1
 *   2. HAL_CAN_Start 启动外设
 *   3. HAL_CAN_ActivateNotification 使能 FIFO1 消息挂起中断
 */
void bsp_can_init(void)
{
    CAN_FilterTypeDef filter;

    /* 诊断：Init 之后、任何操作之前的 CAN 外设状态
    printf("CAN after-Init State=%d (0=RESET 1=READY 2=LISTEN 5=ERROR)\r\n", (int)hcan.State); */

    filter.FilterActivation      = ENABLE;
    filter.FilterMode            = CAN_FILTERMODE_IDMASK;
    filter.FilterScale           = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh          = (0x110U << 5);
    filter.FilterIdLow           = 0x0000U;
    filter.FilterMaskIdHigh      = 0x0000U;   /* 测试：接收所有帧，排除滤波器问题 */
    filter.FilterMaskIdLow       = 0x0000U;
    filter.FilterBank            = 0;
    filter.FilterFIFOAssignment  = CAN_RX_FIFO1;

    HAL_StatusTypeDef st1 = HAL_CAN_ConfigFilter(&hcan, &filter);
    /* printf("CAN MSR before Start=0x%lx (bit0=INAK)\r\n", (unsigned long)hcan.Instance->MSR); */
    HAL_StatusTypeDef st2 = HAL_CAN_Start(&hcan);
    /* printf("CAN MSR after Start=0x%lx Start=%d\r\n", (unsigned long)hcan.Instance->MSR, (int)st2); */
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

    /* 记录状态，供周期打印查看 */
    can_init_state  = (uint32_t)hcan.State;   /* 0=RESET 1=READY 2=LISTENING */
    can_cfg_status  = (uint32_t)st1;
    can_start_status = (uint32_t)st2;

    if (st1 != HAL_OK) can_tx_fail = 0x80000001;  /* 滤波器配置失败标记 */
    if (st2 != HAL_OK) can_tx_fail = 0x80000002;  /* Start 失败标记 */
}

/* 诊断全局量：CAN 初始化状态（供串口周期打印） */
volatile uint32_t can_init_state  = 0;
volatile uint32_t can_cfg_status  = 0;
volatile uint32_t can_start_status = 0;

/* 诊断：读取 CAN 邮箱剩余数量（0 表示邮箱满/发送卡死） */
uint32_t can_mailbox_free(void)
{
    return HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
}

/**
 * @brief 发送一帧标准数据帧
 */
void bsp_can_send(uint32_t std_id, uint8_t *data, uint8_t len)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t mailbox;

    tx_header.StdId = std_id;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = len;
    tx_header.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &mailbox) == HAL_OK)
        can_tx_ok++;
    else
        can_tx_fail++;
}

/**
 * @brief CAN 错误恢复：总线关闭(bus-off)或发送邮箱长时间满时复位
 */
void bsp_can_error_recover(void)
{
    if ((hcan.Instance->ESR & CAN_ESR_BOFF) != 0U)
    {
        can_bus_off_count++;
        HAL_CAN_Stop(&hcan);
        hcan.ErrorCode = HAL_CAN_ERROR_NONE;
        HAL_CAN_Start(&hcan);
        HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
    }
}
