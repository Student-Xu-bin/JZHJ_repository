#include "device.h"
#include "dev_ws2812b.h"

struct device_hw_t ws2812b1_hw_info[] = {
	[0] = {
		.name 			= "full_light1",
		.port 			= FULL_LED_1_GPIO,
		.pin  			= FULL_LED_1_PIN,
		.port_rcu 		= FULL_LED_1_GPIO_RCU,
		.private_data 	= 16,
	},
	{},
};

struct device_hw_t ws2812b2_hw_info[] = {
	[0] = {
		.name 		= "full_light2",
		.port 		= FULL_LED_2_GPIO,
		.pin  		= FULL_LED_2_PIN,
		.port_rcu 	= FULL_LED_2_GPIO_RCU,
		.private_data 	= 16,
	},
	{},
};

struct device_hw_t ws2812b3_hw_info[] = {
	[0] = {
		.name 		= "full_light3",
		.port 		= FULL_LED_3_GPIO,
		.pin  		= FULL_LED_3_PIN,
		.port_rcu 	= FULL_LED_3_GPIO_RCU,
		.private_data 	= 16,
	},
	{},
};

struct device_hw_t ws2812b4_hw_info[] = {
	[0] = {
		.name 		= "full_light4",
		.port 		= FULL_LED_4_GPIO,
		.pin  		= FULL_LED_4_PIN,
		.port_rcu 	= FULL_LED_4_GPIO_RCU,
		.private_data 	= 16,
	},
	{},
};

struct device_hw_t ws2812b5_hw_info[] = {
	[0] = {
		.name 		= "full_light5",
		.port 		= FULL_LED_5_GPIO,
		.pin  		= FULL_LED_5_PIN,
		.port_rcu 	= FULL_LED_5_GPIO_RCU,
		.private_data 	= 16,
	},
	{},
};

struct device_hw_t ws2812b6_hw_info[] = {
	[0] = {
		.name 		= "full_light6",
		.port 		= FULL_LED_6_GPIO,
		.pin  		= FULL_LED_6_PIN,
		.port_rcu 	= FULL_LED_6_GPIO_RCU,
		.private_data 	= 16,
	},
	{},
};

static device_t ws2812b1_dev = {
	.name = "full_light1",
	.driver_name = "full_light",
	.hw_info = ws2812b1_hw_info,
};

static device_t ws2812b2_dev = {
	.name = "full_light2",
	.driver_name = "full_light",
	.hw_info = ws2812b2_hw_info,
};

static device_t ws2812b3_dev = {
	.name = "full_light3",
	.driver_name = "full_light",
	.hw_info = ws2812b3_hw_info,
};

static device_t ws2812b4_dev = {
	.name = "full_light4",
	.driver_name = "full_light",
	.hw_info = ws2812b4_hw_info,
};

static device_t ws2812b5_dev = {
	.name = "full_light5",
	.driver_name = "full_light",
	.hw_info = ws2812b5_hw_info,
};

static device_t ws2812b6_dev = {
	.name = "full_light6",
	.driver_name = "full_light",
	.hw_info = ws2812b6_hw_info,
};

void full_light_register(void)
{
	device_register(&ws2812b1_dev);
	device_register(&ws2812b2_dev);
	device_register(&ws2812b3_dev);
	device_register(&ws2812b4_dev);
	device_register(&ws2812b5_dev);
	device_register(&ws2812b6_dev);
}
INIT_DEVICE_EXPORT(full_light_register)
