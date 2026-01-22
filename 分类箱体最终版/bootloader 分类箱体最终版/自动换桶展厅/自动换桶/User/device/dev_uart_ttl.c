#include "device.h"
#include "dev_uart_ttl.h"

static struct device_hw_t ttl[] = {
	[0] = {
		.name = "uart_no",
		.port = KEY_485_USART,
		.port_rcu = KEY_485_RCU_USART,
		.private_data = KEY_485_BAUDRATE,
	},
	[1] = {
		.name = "uart_rx",
		.port = KEY_485_GPIO,
		.pin  = KEY_485_RX_PIN,
		.port_rcu = KEY_485_GPIO_RCU,
	},
	[2] = {
		.name = "uart_tx",
		.port = KEY_485_GPIO,
		.pin  = KEY_485_TX_PIN,
		.port_rcu = KEY_485_GPIO_RCU,
	},
	{}
};

static device_t dev_ttl = {
	.name = "uart_ttl",
	.driver_name = "uart_ttl",
	.hw_info = ttl,
};

void uart_ttl_device_register(void)
{
	device_register(&dev_ttl);
}
INIT_DEVICE_EXPORT(uart_ttl_device_register)
