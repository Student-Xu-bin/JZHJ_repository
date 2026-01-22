#include "device.h"
#include "dev_led.h"

static struct device_hw_t led[] = {
	[0] = {
		.name = "user_led",
		.port = USER_LED_GPIO,
		.pin = USER_LED_PIN,
		.port_rcu = USER_LED_GPIO_RCU,
	},
	{}
};

static device_t led_device = {
	.name = "bsp_led",
	.driver_name = "bsp_led",
	.hw_info = led,
};

void led_device_register(void)
{
	device_register(&led_device);
}
INIT_DEVICE_EXPORT(led_device_register)
