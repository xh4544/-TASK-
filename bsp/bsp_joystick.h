#ifndef BSP_JOYSTICK_H
#define BSP_JOYSTICK_H

#include "struct_typedef.h"

void bsp_joystick_init(void);
void bsp_joystick_read(int16_t *x, int16_t *y);   /* x,y: -1000 ~ +1000 */

#endif /* BSP_JOYSTICK_H */
