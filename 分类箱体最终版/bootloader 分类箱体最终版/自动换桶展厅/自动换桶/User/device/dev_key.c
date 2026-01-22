#include "dev_key.h"
#include "device.h"
#include "gd32f30x.h"

struct device_hw_t key1_hw_info[] = {
	[0] = {
		.name 		= "key1_detect",
		.port 		= KEY_1_GPIO,
		.pin  		= KEY_1_PIN,
		.port_rcu 	= KEY_1_GPIO_RCU
	},
	{}
};

struct device_hw_t key2_hw_info[] = {
	[0] = {
		.name 		= "key2_detect",
		.port 		= KEY_2_GPIO,
		.pin  		= KEY_2_PIN,
		.port_rcu 	= KEY_2_GPIO_RCU
	},
	{}
};

struct device_hw_t key3_hw_info[] = {
	[0] = {
		.name 		= "key3_detect",
		.port 		= KEY_3_GPIO,
		.pin  		= KEY_3_PIN,
		.port_rcu 	= KEY_3_GPIO_RCU
	},
	{}
};

struct device_hw_t key4_hw_info[] = {
	[0] = {
		.name 		= "key4_detect",
		.port 		= KEY_4_GPIO,
		.pin  		= KEY_4_PIN,
		.port_rcu 	= KEY_4_GPIO_RCU
	},
	{}
};

struct device_hw_t key5_hw_info[] = {
	[0] = {
		.name 		= "key5_detect",
		.port 		= KEY_5_GPIO,
		.pin  		= KEY_5_PIN,
		.port_rcu 	= KEY_5_GPIO_RCU
	},
	{}
};

struct device_hw_t key6_hw_info[] = {
	[0] = {
		.name 		= "key6_detect",
		.port 		= KEY_6_GPIO,
		.pin  		= KEY_6_PIN,
		.port_rcu 	= KEY_6_GPIO_RCU
	},
	{}
};

static device_t key1_dev = {
	.name = "key1",
	.driver_name = "key",
	.hw_info = key1_hw_info,
};

static device_t key2_dev = {
	.name = "key2",
	.driver_name = "key",
	.hw_info = key2_hw_info,
};

static device_t key3_dev = {
	.name = "key3",
	.driver_name = "key",
	.hw_info = key3_hw_info,
};

static device_t key4_dev = {
	.name = "key4",
	.driver_name = "key",
	.hw_info = key4_hw_info,
};

static device_t key5_dev = {
	.name = "key5",
	.driver_name = "key",
	.hw_info = key5_hw_info,
};

static device_t key6_dev = {
	.name = "key6",
	.driver_name = "key",
	.hw_info = key6_hw_info,
};

void keys_register(void)
{
	device_register(&key1_dev);
	device_register(&key2_dev);
	device_register(&key3_dev);
	device_register(&key4_dev);
	device_register(&key5_dev);
	device_register(&key6_dev);
}
INIT_DEVICE_EXPORT(keys_register)
