#include "device.h"
#include "dev_motor.h"
#include "gd32f30x.h"

struct device_hw_t motor1_hw_info[] = {
	[0] = {
		.name 		= "motor1_ctl_1",
		.port 		= MOTOR1_CTL_PORT1,
		.pin  		= MOTOR1_CTL_PIN1,
		.port_rcu 	= MOTOR1_CTL1_PORT_RCU
	},
	[1] = {
		.name 		= "motor1_ctl_2",
		.port 		= MOTOR1_CTL_PORT2,
		.pin  		= MOTOR1_CTL_PIN2,
		.port_rcu 	= MOTOR1_CTL2_PORT_RCU
	},
};

struct device_hw_t motor2_hw_info[] = {
	[0] = {
		.name 		= "motor2_ctl_1",
		.port 		= MOTOR2_CTL_PORT1,
		.pin  		= MOTOR2_CTL_PIN1,
		.port_rcu 	= MOTOR2_CTL1_PORT_RCU
	},
	[1] = {
		.name 		= "motor2_ctl_2",
		.port 		= MOTOR2_CTL_PORT2,
		.pin  		= MOTOR2_CTL_PIN2,
		.port_rcu 	= MOTOR2_CTL2_PORT_RCU
	},
};

struct device_hw_t motor3_hw_info[] = {
	[0] = {
		.name 		= "motor3_ctl_1",
		.port 		= MOTOR3_CTL_PORT1,
		.pin  		= MOTOR3_CTL_PIN1,
		.port_rcu 	= MOTOR3_CTL1_PORT_RCU
	},
	[1] = {
		.name 		= "motor3_ctl_2",
		.port 		= MOTOR3_CTL_PORT2,
		.pin  		= MOTOR3_CTL_PIN2,
		.port_rcu 	= MOTOR3_CTL2_PORT_RCU
	},
};

struct device_hw_t motor4_hw_info[] = {
	[0] = {
		.name 		= "motor4_ctl_1",
		.port 		= MOTOR4_CTL_PORT1,
		.pin  		= MOTOR4_CTL_PIN1,
		.port_rcu 	= MOTOR4_CTL1_PORT_RCU
	},
	[1] = {
		.name 		= "motor4_ctl_2",
		.port 		= MOTOR4_CTL_PORT2,
		.pin  		= MOTOR4_CTL_PIN2,
		.port_rcu 	= MOTOR1_CTL2_PORT_RCU
	},
};

struct device_hw_t motor5_hw_info[] = {
	[0] = {
		.name 		= "motor5_ctl_1",
		.port 		= MOTOR5_CTL_PORT1,
		.pin  		= MOTOR5_CTL_PIN1,
		.port_rcu 	= MOTOR5_CTL1_PORT_RCU
	},
	[1] = {
		.name 		= "motor5_ctl_2",
		.port 		= MOTOR5_CTL_PORT2,
		.pin  		= MOTOR5_CTL_PIN2,
		.port_rcu 	= MOTOR5_CTL2_PORT_RCU
	},
};

struct device_hw_t motor6_hw_info[] = {
	[0] = {
		.name 		= "motor6_ctl_1",
		.port 		= MOTOR6_CTL_PORT1,
		.pin  		= MOTOR6_CTL_PIN1,
		.port_rcu 	= MOTOR6_CTL1_PORT_RCU
	},
	[1] = {
		.name 		= "motor6_ctl_2",
		.port 		= MOTOR6_CTL_PORT2,
		.pin  		= MOTOR6_CTL_PIN2,
		.port_rcu 	= MOTOR6_CTL2_PORT_RCU
	},
};

static device_t motor1_dev = {
	.name = "motor1",
	.driver_name = "motor",
	.hw_info = motor1_hw_info,
};

static device_t motor2_dev = {
	.name = "motor2",
	.driver_name = "motor",
	.hw_info = motor2_hw_info,
};

static device_t motor3_dev = {
	.name = "motor3",
	.driver_name = "motor",
	.hw_info = motor3_hw_info,
};

static device_t motor4_dev = {
	.name = "motor4",
	.driver_name = "motor",
	.hw_info = motor4_hw_info,
};

static device_t motor5_dev = {
	.name = "motor5",
	.driver_name = "motor",
	.hw_info = motor5_hw_info,
};

static device_t motor6_dev = {
	.name = "motor6",
	.driver_name = "motor",
	.hw_info = motor6_hw_info,
};

void motors_register(void)
{
	device_register(&motor1_dev);
	device_register(&motor2_dev);
	device_register(&motor3_dev);
	device_register(&motor4_dev);
	device_register(&motor5_dev);
	device_register(&motor6_dev);
}
INIT_DEVICE_EXPORT(motors_register)
