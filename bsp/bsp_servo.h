#ifndef BSP_SERVO_H
#define BSP_SERVO_H

#include "struct_typedef.h"

void bsp_servo_init(void);
void bsp_servo_set_speed(int16_t speed);   /* speed: -1000 ~ +1000 */

#endif /* BSP_SERVO_H */
