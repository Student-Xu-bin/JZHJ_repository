#ifndef __RS485_H__
#define __RS485_H__

#include "gd32f30x.h"

#define BAUDRATE 	115200U

#define _receive_mode()    	_74hc595_ctl(RS485_MASTER, RS485_RECEIVE, OFF, 1)
#define _send_mode()    	_74hc595_ctl(RS485_MASTER, RS485_SEND, OFF, 1)

typedef struct
{
	uint32_t uartNo;
	rcu_periph_enum rcuUart;
	rcu_periph_enum rcuGpio;
	rcu_periph_enum rcuCtlGpio;
	uint32_t gpio;
	uint32_t ctlGpio;
	uint32_t txPin;
	uint32_t rxPin;
	uint32_t ctlPin;
	uint8_t irq;
	
} UartHwInfo_t;

void rs485_init(void);
int rs485_read_byte(uint8_t *data);
int ttl_read_byte(uint8_t *data);
int32_t ReceiveByteTimeout(uint8_t *c, uint32_t timeout);
void SendByte(uint8_t c);
void ttl_sendbyte(uint8_t data[], int size);

#endif
