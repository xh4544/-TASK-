#include "bsp_led.h"
#include "main.h"

/* PC13 板载 LED 低电平点亮 */

static void bsp_delay_us(uint32_t us)
{
    if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0U)
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
    }

    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < ticks)
    {
    }
}

/**
 * @brief 熄灭 LED
 */
void bsp_led_off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/**
 * @brief 翻转 LED 状态
 */
void bsp_led_toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

/**
 * @brief 呼吸灯
 */
void bsp_led_breathing_tick(void)
{
    static uint8_t brightness = 0;
    static int8_t  dir = 1;
    uint8_t i;

    brightness += dir;
    if (brightness >= 100) dir = -1;
    if (brightness == 0)   dir = 1;

    for (i = 0; i < 100; i++)
    {
        GPIO_PinState s = (i < brightness) ? GPIO_PIN_RESET : GPIO_PIN_SET;
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, s);
        bsp_delay_us(20);
    }
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}
