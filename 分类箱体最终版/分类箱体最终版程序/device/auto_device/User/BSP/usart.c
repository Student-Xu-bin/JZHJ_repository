#include "usart.h"

static void ttl_gpio_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
		
	gpio_bit_reset(GPIOC, GPIO_PIN_10);
	gpio_bit_reset(GPIOC, GPIO_PIN_11);
	
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
}

void ttl_init(void)
{
	ttl_gpio_init();
	
	rcu_periph_clock_enable(RCU_UART3);
	
	usart_deinit(UART3);
	usart_baudrate_set(UART3, 115200U);
	usart_word_length_set(UART3, USART_WL_8BIT);
	usart_stop_bit_set(UART3, USART_STB_1BIT);
	usart_parity_config(UART3, USART_PM_NONE);
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
	
	usart_enable(UART3);
}

void ttl_send_data(uint8_t *buf, uint16_t len)
{
	uint8_t *tmp_data = (uint8_t *)buf;
	
	for(int i = 0; i < len; i ++)
	{
		usart_data_transmit(UART3, (uint8_t)tmp_data[i]);
		while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));
	}
}

