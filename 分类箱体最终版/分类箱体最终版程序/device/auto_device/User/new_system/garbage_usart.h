#ifndef __GARBAGE_USART_H
#define __GARBAGE_USART_H

#include "gd32f30x.h"

class garbage_usart
{
	public:
		garbage_usart(uint32_t usart_no, uint32_t baudrate);
		garbage_usart() = default;
		void usart_send_data(uint8_t *buf, uint8_t len);
	private:
		uint32_t usart_port;
};

#endif
