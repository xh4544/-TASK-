#include "bsp_servo.h"
#include "tim.h"

/* TIM2 时钟 72MHz，Prescaler=71 -> 1 tick = 1us，Period=19999 -> 20ms(50Hz) */
#define SERVO_STOP_PULSE   1500   /* 1.5ms 停止 */
#define SERVO_MIN_PULSE    1000   /* 1.0ms 满速反转 */
#define SERVO_MAX_PULSE    2000   /* 2.0ms 满速正转 */

/**
 * @brief 启动舵机 PWM，并输出停止位
 */
void bsp_servo_init(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    bsp_servo_set_speed(0);
}

/**
 * @brief 设置 360° 舵机转速
 * @param speed -1000~+1000，0 停止，正负表示方向
 */
void bsp_servo_set_speed(int16_t speed)
{
    int32_t pulse;

    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;

    /* speed -> 脉宽 1000~2000us */
    pulse = SERVO_STOP_PULSE + (int32_t)speed * 500 / 1000;

    if (pulse > SERVO_MAX_PULSE) pulse = SERVO_MAX_PULSE;
    if (pulse < SERVO_MIN_PULSE) pulse = SERVO_MIN_PULSE;

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, (uint16_t)pulse);
}
