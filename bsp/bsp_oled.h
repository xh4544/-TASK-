#ifndef BSP_OLED_H
#define BSP_OLED_H

#include "struct_typedef.h"

void bsp_oled_init(void);
void bsp_oled_clear(void);
void bsp_oled_show_str(uint8_t x, uint8_t y, const char *str);   /* x:0~127, y:页0~7 */
void bsp_oled_show_num(uint8_t x, uint8_t y, int32_t num, uint8_t len);
void bsp_oled_flush(void);   /* 将显存整体刷新到屏幕 */

#endif /* BSP_OLED_H */
