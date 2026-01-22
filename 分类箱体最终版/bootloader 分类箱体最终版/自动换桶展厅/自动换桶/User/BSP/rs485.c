#include <stdio.h>
#include "rs485.h"
#include "74hc595.h"

#define _receive_mode()    	_74hc595_ctl(RS485_MASTER, RS485_RECEIVE, OFF, 1)
#define _send_mode()    	_74hc595_ctl(RS485_MASTER, RS485_SEND, OFF, 1)

static UartHwInfo_t ttl_hw_info = {UART3, RCU_UART3, RCU_GPIOC, RCU_GPIOE, GPIOC, GPIOE, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_10, UART3_IRQn};
static UartHwInfo_t g_uartHwInfo = {USART0, RCU_USART0, RCU_GPIOA, RCU_GPIOA, GPIOA, GPIOA, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, USART0_IRQn};

static void rs485_gpio_init(void)
{
    rcu_periph_clock_enable(g_uartHwInfo.rcuGpio);
	
	rcu_periph_clock_enable(ttl_hw_info.rcuGpio);
	
//	gpio_bit_reset(ttl_hw_info.gpio, ttl_hw_info.txPin);
	
	gpio_init(g_uartHwInfo.gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, g_uartHwInfo.txPin);
	gpio_init(g_uartHwInfo.gpio, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, g_uartHwInfo.rxPin);
	
	gpio_init(ttl_hw_info.gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, ttl_hw_info.txPin);
	
//	gpio_bit_reset(ttl_hw_info.gpio, ttl_hw_info.txPin);
	gpio_init(ttl_hw_info.gpio, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, ttl_hw_info.rxPin);
	
}

int _rs485_init(uint32_t baudrate)
{
	rcu_periph_clock_enable(g_uartHwInfo.rcuUart);
	usart_deinit (g_uartHwInfo.uartNo);
	usart_baudrate_set(g_uartHwInfo.uartNo, baudrate);
	usart_transmit_config(g_uartHwInfo.uartNo, USART_TRANSMIT_ENABLE);
	usart_receive_config(g_uartHwInfo.uartNo, USART_RECEIVE_ENABLE);
	usart_flag_clear(g_uartHwInfo.uartNo, USART_FLAG_RBNE);
	usart_flag_clear(g_uartHwInfo.uartNo,USART_FLAG_TC);
	usart_enable(g_uartHwInfo.uartNo);
	
	rcu_periph_clock_enable(ttl_hw_info.rcuUart);
	usart_deinit (ttl_hw_info.uartNo);
	usart_baudrate_set(ttl_hw_info.uartNo, baudrate);
	usart_transmit_config(ttl_hw_info.uartNo, USART_TRANSMIT_ENABLE);
	usart_receive_config(ttl_hw_info.uartNo, USART_RECEIVE_ENABLE);
	usart_flag_clear(ttl_hw_info.uartNo,USART_FLAG_TBE);
	usart_enable(ttl_hw_info.uartNo);
	
	return 1;
}

void rs485_init(void)
{
	_rs485_init(BAUDRATE);
	rs485_gpio_init();
}

int fputc(int ch, FILE *f)
{
	while (RESET == usart_flag_get(ttl_hw_info.uartNo, USART_FLAG_TBE));
	USART_DATA(ttl_hw_info.uartNo) = USART_DATA_DATA & (uint32_t)ch;
	
	
	return ch;
}

int rs485_read_byte(uint8_t *data)
{
	if(usart_flag_get(g_uartHwInfo.uartNo, USART_FLAG_RBNE) != RESET)
	{
		*data = (uint8_t)usart_data_receive(g_uartHwInfo.uartNo);
		return 1;
	}
	else
	{
		return 0;
	}
}


int32_t ReceiveByteTimeout(uint8_t *c, uint32_t timeout)
{
	while (timeout-- > 0)
	{
		if (rs485_read_byte(c))
		{
			return 0;
		}
	}
	return -1;
}

void SendByte(uint8_t c)
{
	_send_mode();
	usart_data_transmit(USART0, c);
	while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
	
	_receive_mode();
}

void ttl_sendbyte(uint8_t data[], int size)
{
	if(!data)
		return;
	
	extern void delay_nms(uint32_t nMs);
	for(int i = 0; i < size; i ++){
		printf("%X",data[i]);
		//delay_nms(10);
	}
	printf("\r\n");
	delay_nms(10);
}
