#include "device.h"
#include "dev_adc.h"

static struct device_hw_t adc_channel_1[] = {
	[0] = {
		.name = "adc_channel_1",
		.port = ADC_1_GPIO,
		.pin  = ADC_1_PIN,
		.port_rcu = ADC_1_RCU_GPIO,
		.private_data = ADC_CHANNEL_8,
	},
	{}
};

static struct device_hw_t adc_channel_2[] = {
	[0] = {
		.name = "adc_channel_2",
		.port = ADC_2_GPIO,
		.pin  = ADC_2_PIN,
		.port_rcu = ADC_2_RCU_GPIO,
		.private_data = ADC_CHANNEL_9,
	},
	{}
};

static struct device_hw_t adc_channel_3[] = {
	[0] = {
		.name = "adc_channel_3",
		.port = ADC_3_GPIO,
		.pin  = ADC_3_PIN,
		.port_rcu = ADC_3_RCU_GPIO,
		.private_data = ADC_CHANNEL_0,
	},
	{}
};

static struct device_hw_t adc_channel_4[] = {
	[0] = {
		.name = "adc_channel_4",
		.port = ADC_4_GPIO,
		.pin  = ADC_4_PIN,
		.port_rcu = ADC_4_RCU_GPIO,
		.private_data = ADC_CHANNEL_1,
	},
	{}
};

static struct device_hw_t adc_channel_5[] = {
	[0] = {
		.name = "adc_channel_5",
		.port = ADC_5_GPIO,
		.pin  = ADC_5_PIN,
		.port_rcu = ADC_5_RCU_GPIO,
		.private_data = ADC_CHANNEL_15,
	},
	{}
};

static struct device_hw_t adc_channel_6[] = {
	[0] = {
		.name = "adc_channel_6",
		.port = ADC_6_GPIO,
		.pin  = ADC_6_PIN,
		.port_rcu = ADC_6_RCU_GPIO,
		.private_data = ADC_CHANNEL_14,
	},
	{}
};

static device_t device_adc_channel_1 = {
	.name = "adc_channel_1",
	.driver_name = "adc_channel",
	.hw_info = adc_channel_1,
};

static device_t device_adc_channel_2 = {
	.name = "adc_channel_2",
	.driver_name = "adc_channel",
	.hw_info = adc_channel_2,
};

static device_t device_adc_channel_3 = {
	.name = "adc_channel_3",
	.driver_name = "adc_channel",
	.hw_info = adc_channel_3,
};

static device_t device_adc_channel_4 = {
	.name = "adc_channel_4",
	.driver_name = "adc_channel",
	.hw_info = adc_channel_4,
};

static device_t device_adc_channel_5 = {
	.name = "adc_channel_5",
	.driver_name = "adc_channel",
	.hw_info = adc_channel_5,
};

static device_t device_adc_channel_6 = {
	.name = "adc_channel_6",
	.driver_name = "adc_channel",
	.hw_info = adc_channel_6,
};

void adc_channel_device_register(void)
{
	device_register(&device_adc_channel_1);
	device_register(&device_adc_channel_2);
	device_register(&device_adc_channel_3);
	device_register(&device_adc_channel_4);
	device_register(&device_adc_channel_5);
	device_register(&device_adc_channel_6);
}
INIT_DEVICE_EXPORT(adc_channel_device_register)
