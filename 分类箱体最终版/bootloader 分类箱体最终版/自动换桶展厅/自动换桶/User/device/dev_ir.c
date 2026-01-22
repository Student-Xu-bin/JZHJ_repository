#include "dev_ir.h"
#include "device.h"
#include "gd32f30x.h"

struct device_hw_t ir1_hw_info[] = {
	[0] = {
		.name 		= "ir1_detect",
		.port 		= INFRARED_1_DET_GPIO,
		.pin  		= INFRARED_1_DET_PIN,
		.port_rcu 	= INFRARED_1_DET_GPIO_RCU
	},
	{}
};

struct device_hw_t ir2_hw_info[] = {
	[0] = {
		.name 		= "ir2_detect",
		.port 		= INFRARED_2_DET_GPIO,
		.pin  		= INFRARED_2_DET_PIN,
		.port_rcu 	= INFRARED_2_DET_GPIO_RCU
	},
	{}
};

struct device_hw_t ir3_hw_info[] = {
	[0] = {
		.name 		= "ir3_detect",
		.port 		= INFRARED_3_DET_GPIO,
		.pin  		= INFRARED_3_DET_PIN,
		.port_rcu 	= INFRARED_3_DET_GPIO_RCU
	},
	{}
};

struct device_hw_t ir4_hw_info[] = {
	[0] = {
		.name 		= "ir4_detect",
		.port 		= INFRARED_4_DET_GPIO,
		.pin  		= INFRARED_4_DET_PIN,
		.port_rcu 	= INFRARED_4_DET_GPIO_RCU
	},
	{}
};

struct device_hw_t ir5_hw_info[] = {
	[0] = {
		.name 		= "ir5_detect",
		.port 		= INFRARED_5_DET_GPIO,
		.pin  		= INFRARED_5_DET_PIN,
		.port_rcu 	= INFRARED_5_DET_GPIO_RCU
	},
	{}
};

struct device_hw_t ir6_hw_info[] = {
	[0] = {
		.name 		= "ir6_detect",
		.port 		= INFRARED_6_DET_GPIO,
		.pin  		= INFRARED_6_DET_PIN,
		.port_rcu 	= INFRARED_6_DET_GPIO_RCU
	},
	{}
};

static device_t ir1_dev = {
	.name = "ir1",
	.driver_name = "ir",
	.hw_info = ir1_hw_info,
};

static device_t ir2_dev = {
	.name = "ir2",
	.driver_name = "ir",
	.hw_info = ir2_hw_info,
};

static device_t ir3_dev = {
	.name = "ir3",
	.driver_name = "ir",
	.hw_info = ir3_hw_info,
};

static device_t ir4_dev = {
	.name = "ir4",
	.driver_name = "ir",
	.hw_info = ir4_hw_info,
};

static device_t ir5_dev = {
	.name = "ir5",
	.driver_name = "ir",
	.hw_info = ir5_hw_info,
};

static device_t ir6_dev = {
	.name = "ir6",
	.driver_name = "ir",
	.hw_info = ir6_hw_info,
};

void irs_register(void)
{
	device_register(&ir1_dev);
	device_register(&ir2_dev);
	device_register(&ir3_dev);
	device_register(&ir4_dev);
	device_register(&ir5_dev);
	device_register(&ir6_dev);
}
INIT_DEVICE_EXPORT(irs_register)
