#ifndef BSP_OLED_I2C_H
#define BSP_OLED_I2C_H

#include "struct_typedef.h"

void bsp_oled_i2c_init(void);
void bsp_oled_i2c_clear(void);
void bsp_oled_i2c_show_str(uint8_t x, uint8_t page, const char *str);
void bsp_oled_i2c_show_num(uint8_t x, uint8_t page, int32_t num, uint8_t len);
void bsp_oled_i2c_flush(void);

#endif /* BSP_OLED_I2C_H */
