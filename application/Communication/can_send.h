#ifndef CAN_SEND_H
#define CAN_SEND_H

#include "struct_typedef.h"

void gimbal_send_control(int16_t motor_target, int16_t servo_target);
void chassis_send_status(int16_t motor_actual, uint8_t motor_online);

#endif /* CAN_SEND_H */
