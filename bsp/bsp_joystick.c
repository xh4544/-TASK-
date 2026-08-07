#include "bsp_joystick.h"
#include "adc.h"

/*
 *   PA0 = ADC_IN0 -> 摇杆 X 轴
 *   PA1 = ADC_IN1 -> 摇杆 Y 轴
 */
#define JOY_CENTER    2048    /* 12 位 ADC 中点 */
#define JOY_DEADZONE  150     /* 死区 */

static int16_t map_axis(uint16_t raw)
{
    int32_t v = (int32_t)raw - JOY_CENTER;

    if (v > -JOY_DEADZONE && v < JOY_DEADZONE)
        return 0;

    if (v >= JOY_DEADZONE)
        v = (v - JOY_DEADZONE) * 1000 / (JOY_CENTER - JOY_DEADZONE);
    else
        v = (v + JOY_DEADZONE) * 1000 / (JOY_CENTER - JOY_DEADZONE);

    if (v > 1000)  v = 1000;
    if (v < -1000) v = -1000;

    return (int16_t)v;
}

void bsp_joystick_init(void)
{
    /* ADC 已在 MX_ADC1_Init 中初始化 */
}

void bsp_joystick_read(int16_t *x, int16_t *y)
{
    uint16_t vx, vy;

    /* 扫描模式：rank1=CH0, rank2=CH1 */
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    vx = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    vy = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    *x = map_axis(vx);
    *y = map_axis(vy);
}
