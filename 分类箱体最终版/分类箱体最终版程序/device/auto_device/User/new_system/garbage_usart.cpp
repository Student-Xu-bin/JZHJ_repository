#include "garbage_usart.h"

garbage_usart::garbage_usart(uint32_t usart_no, uint32_t baudrate)
{
	usart_port = usart_no;
	if(usart_port == UART3)
	{
		rcu_periph_clock_enable(RCU_GPIOC);
		
		gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_11);
		gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
		
		usart_deinit(UART3);
		usart_baudrate_set(UART3, baudrate);
		usart_word_length_set(UART3, USART_WL_8BIT);
		usart_stop_bit_set(UART3, USART_STB_1BIT);
		usart_parity_config(UART3, USART_PM_NONE);
		usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
		usart_receive_config(UART3, USART_RECEIVE_ENABLE);

		usart_enable(UART3);
	}
}

void garbage_usart::usart_send_data(uint8_t *buf, uint8_t len)
{
	while(len--)
	{
		usart_data_transmit(usart_port, *buf++);
		while(RESET == usart_flag_get(USART1, USART_FLAG_TC));
	}
}