#ifndef __DRV_MOTOR_H
#define __DRV_MOTOR_H

#include "dev_motor.h"

void motor_init(void);
void motor_stop(motor_t *p);
void motor_open(motor_t *p);
void motor_close(motor_t *p);

#endif
