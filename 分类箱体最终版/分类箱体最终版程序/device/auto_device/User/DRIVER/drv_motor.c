#include "drv_motor.h"

void motor_init(void)
{
	for(int i = 0; i < 6; i++)
		motor_gpio_init(&motor[i]);
}

void motor_stop(motor_t *p)
{
	if(!p)
		return;
	
	gpio_bit_reset(p->port_1, p->pin_1);
	gpio_bit_reset(p->port_2, p->pin_2);
	
	p->state = MOTOR_STOP;
}

void motor_open(motor_t *p)
{
	if(!p)
		return;
	
	motor_stop(p);
	
	gpio_bit_reset(p->port_2, p->pin_2);
	gpio_bit_set(p->port_1, p->pin_1);
	
	p->state = MOTOR_OPEN;
}

void motor_close(motor_t *p)
{
	if(!p)
		return;
	
	motor_stop(p);
	
	gpio_bit_reset(p->port_1, p->pin_1);
	gpio_bit_set(p->port_2, p->pin_2);
	
	p->state = MOTOR_CLOSE;
}


