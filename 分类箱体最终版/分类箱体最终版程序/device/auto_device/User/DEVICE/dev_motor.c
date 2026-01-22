#include "dev_motor.h"

motor_t motor[6] = {
	[0] = {
		.rcu_port_1 = MOTOR1_CTL1_PORT_RCU,
		.rcu_port_2 = MOTOR1_CTL2_PORT_RCU,
		.port_1 = MOTOR1_CTL_PORT1,
		.port_2 = MOTOR1_CTL_PORT2,
		.pin_1 = MOTOR1_CTL_PIN1,
		.pin_2 = MOTOR1_CTL_PIN2,
		.state = MOTOR_STOP,
		.zero_current = 0,
		.limit_currnet = 0,
		.no = 0,
		.open_state = 0,
		.close_state = 0,
		.channel_init_flag = 0,
		.error_flag = 0
	},
	[1] = {
		.rcu_port_1 = MOTOR2_CTL1_PORT_RCU,
		.rcu_port_2 = MOTOR2_CTL2_PORT_RCU,
		.port_1 = MOTOR2_CTL_PORT1,
		.port_2 = MOTOR2_CTL_PORT2,
		.pin_1 = MOTOR2_CTL_PIN1,
		.pin_2 = MOTOR2_CTL_PIN2,
		.state = MOTOR_STOP,
		.zero_current = 0,
		.limit_currnet = 0,
		.no = 1,
		.open_state = 0,
		.close_state = 0,
		.channel_init_flag = 0,
		.error_flag = 0
	},
	[2] = {
		.rcu_port_1 = MOTOR3_CTL1_PORT_RCU,
		.rcu_port_2 = MOTOR3_CTL2_PORT_RCU,
		.port_1 = MOTOR3_CTL_PORT1,
		.port_2 = MOTOR3_CTL_PORT2,
		.pin_1 = MOTOR3_CTL_PIN1,
		.pin_2 = MOTOR3_CTL_PIN2,
		.state = MOTOR_STOP,
		.zero_current = 0,
		.limit_currnet = 0,
		.no = 2,
		.open_state = 0,
		.close_state = 0,
		.channel_init_flag = 0,
		.error_flag = 0
	},
	[3] = {
		.rcu_port_1 = MOTOR4_CTL1_PORT_RCU,
		.rcu_port_2 = MOTOR4_CTL2_PORT_RCU,
		.port_1 = MOTOR4_CTL_PORT1,
		.port_2 = MOTOR4_CTL_PORT2,
		.pin_1 = MOTOR4_CTL_PIN1,
		.pin_2 = MOTOR4_CTL_PIN2,
		.state = MOTOR_STOP,
		.zero_current = 0,
		.limit_currnet = 0,
		.no = 3,
		.open_state = 0,
		.close_state = 0,
		.channel_init_flag = 0,
		.error_flag = 0
	},
	[4] = {
		.rcu_port_1 = MOTOR5_CTL1_PORT_RCU,
		.rcu_port_2 = MOTOR5_CTL2_PORT_RCU,
		.port_1 = MOTOR5_CTL_PORT1,
		.port_2 = MOTOR5_CTL_PORT2,
		.pin_1 = MOTOR5_CTL_PIN1,
		.pin_2 = MOTOR5_CTL_PIN2,
		.state = MOTOR_STOP,
		.zero_current = 0,
		.limit_currnet = 0,
		.no = 4,
		.open_state = 0,
		.close_state = 0,
		.channel_init_flag = 0,
		.error_flag = 0
	},
	[5] = {
		.rcu_port_1 = MOTOR6_CTL1_PORT_RCU,
		.rcu_port_2 = MOTOR6_CTL2_PORT_RCU,
		.port_1 = MOTOR6_CTL_PORT1,
		.port_2 = MOTOR6_CTL_PORT2,
		.pin_1 = MOTOR6_CTL_PIN1,
		.pin_2 = MOTOR6_CTL_PIN2,
		.state = MOTOR_STOP,
		.zero_current = 0,
		.limit_currnet = 0,
		.no = 5,
		.open_state = 0,
		.close_state = 0,
		.channel_init_flag = 0,
		.error_flag = 0
	},
};

void motor_gpio_init(motor_t *p)
{
	rcu_periph_clock_enable(p->rcu_port_1);
	rcu_periph_clock_enable(p->rcu_port_2);
	
	if(p->no == 1)
	{
		rcu_periph_clock_enable(RCU_AF);
		gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	}
	
	gpio_bit_reset(p->port_1, p->pin_1);
	gpio_bit_reset(p->port_2, p->pin_2);
	
	gpio_init(p->port_1, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, p->pin_1);
	gpio_init(p->port_2, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, p->pin_2);
}




