#include "drv_uart_ttl.h"
#include "driver.h"
#include "device.h"

static int uart_ttl_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	
	
	return count;
}

static int uart_ttl_write(device_t *pdev, const void *buf, size_t count)
{
	if(!pdev)
		return -1;

	uint8_t *tmp_data = (uint8_t *)buf;
	
	for(size_t i = 0; i < count; i ++)
	{
		usart_data_transmit(pdev->hw_info->port, (uint8_t)tmp_data[i]);
		while(RESET == usart_flag_get(pdev->hw_info->port, USART_FLAG_TBE));
	}
	
	return count;
}

static int uart_ttl_close(device_t *pdev)
{
	usart_deinit(pdev->hw_info->port);
	
	return 0;
}

static operations_t uart_ttl_operations = {
	.read = uart_ttl_read,
	.write = uart_ttl_write,
	.close = uart_ttl_close,
};

static void uart_ttl_init(device_t *pdev)
{
	rcu_periph_clock_enable(pdev->hw_info[1].port_rcu);
	rcu_periph_clock_enable(pdev->hw_info[2].port_rcu);
	
	gpio_init(pdev->hw_info[1].port, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, pdev->hw_info[1].pin);
	gpio_init(pdev->hw_info[2].port, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, pdev->hw_info[2].pin);
	
	rcu_periph_clock_enable(pdev->hw_info->port_rcu);
	
	usart_deinit(pdev->hw_info->port);
	usart_baudrate_set(pdev->hw_info->port, pdev->hw_info->private_data);
    usart_word_length_set(pdev->hw_info->port, USART_WL_8BIT);
    usart_stop_bit_set(pdev->hw_info->port, USART_STB_1BIT);
    usart_parity_config(pdev->hw_info->port, USART_PM_NONE);
    usart_transmit_config(pdev->hw_info->port, USART_TRANSMIT_ENABLE);
	
    usart_enable(pdev->hw_info->port);
}

static int uart_ttl_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &uart_ttl_operations;
	
	uart_ttl_init(pdev);
	
	pdev->state = ON;
	
	return 0;
}

static driver_t uart_ttl_driver = {
	.name = "uart_ttl",
	.open = uart_ttl_open,
};

void uart_ttl_driver_init(void)
{
	driver_register(&uart_ttl_driver);
}
INIT_DRIVE_EXPORT(uart_ttl_driver_init)
