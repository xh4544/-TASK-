#include "bsp_motor.h"
#include "tim.h"
#include "main.h"
#define ENCODER_PPR_MOTOR        13
#define GEAR_RATIO               20
#define ENCODER_COUNTS_PER_REV   (ENCODER_PPR_MOTOR * GEAR_RATIO * 4)

/* TIM3 时钟 72MHz，Prescaler=71 -> 1MHz，Period=49 -> 20kHz PWM */
#define MOTOR_PWM_MAX            49

static uint16_t last_cnt  = 0;
static uint32_t last_tick = 0;
static int16_t  last_rpm  = 0;

/**
 * @brief 初始化 TB6612（使能、停转）并启动编码器PWM
 */
void bsp_motor_init(void)
{
    /* 使能 TB6612 */
    HAL_GPIO_WritePin(TB6612_STBY_GPIO_Port, TB6612_STBY_Pin, GPIO_PIN_SET);
    /* 初始停止：AIN1=AIN2=0 */
    HAL_GPIO_WritePin(TB6612_AIN1_GPIO_Port, TB6612_AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TB6612_AIN2_GPIO_Port, TB6612_AIN2_Pin, GPIO_PIN_RESET);

    /* 启动编码器接口与电机 PWM（TIM3_CH3 = PB0） */
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    last_cnt  = (uint16_t)__HAL_TIM_GET_COUNTER(&htim1);
    last_tick = HAL_GetTick();
}

/**
 * @brief 设置电机驱动
 * @param duty -1000~+1000
 */
void bsp_motor_set_duty(int16_t duty)
{
    uint16_t pwm;
    uint16_t abs_duty;

    if (duty > 1000) duty = 1000;
    if (duty < -1000) duty = -1000;

    abs_duty = (uint16_t)((duty < 0) ? -duty : duty);
    pwm = (uint16_t)((uint32_t)abs_duty * MOTOR_PWM_MAX / 1000);

    if (duty > 0)
    {
        /* 正转 */
        HAL_GPIO_WritePin(TB6612_AIN1_GPIO_Port, TB6612_AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(TB6612_AIN2_GPIO_Port, TB6612_AIN2_Pin, GPIO_PIN_RESET);
    }
    else if (duty < 0)
    {
        /* 反转 */
        HAL_GPIO_WritePin(TB6612_AIN1_GPIO_Port, TB6612_AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(TB6612_AIN2_GPIO_Port, TB6612_AIN2_Pin, GPIO_PIN_SET);
    }
    else
    {
        /* 停止 */
        HAL_GPIO_WritePin(TB6612_AIN1_GPIO_Port, TB6612_AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(TB6612_AIN2_GPIO_Port, TB6612_AIN2_Pin, GPIO_PIN_RESET);
    }

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pwm);
}

/**
 * @brief 读取编码器并计算实际转速
 * @return 实际转速 rpm
 */
int16_t bsp_motor_read_speed_rpm(void)
{
    uint32_t now   = HAL_GetTick();
    uint16_t cnt   = (uint16_t)__HAL_TIM_GET_COUNTER(&htim1);
    int16_t  delta = (int16_t)(cnt - last_cnt);   /* 16 位回绕安全，|delta| < 32768 */
    int32_t  dt_ms = (int32_t)(now - last_tick);
    int32_t  rpm;

    last_cnt  = cnt;
    last_tick = now;

    if (dt_ms <= 0)
        return last_rpm;

    /* rpm = delta(计数) / 1040(计数/转) / dt_ms(ms) * 60000 */
    rpm = (int32_t)delta * 60000 / ENCODER_COUNTS_PER_REV / dt_ms;
    last_rpm = (int16_t)rpm;

    return last_rpm;
}
