#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include "struct_typedef.h"

void    bsp_motor_init(void);
void    bsp_motor_set_duty(int16_t duty);        /* duty: -1000 ~ +1000 */
int16_t bsp_motor_read_speed_rpm(void);

#endif /* BSP_MOTOR_H */
