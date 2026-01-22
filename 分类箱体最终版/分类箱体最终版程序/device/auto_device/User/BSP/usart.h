#ifndef __USART_H
#define __USART_H

#include "gd32f30x.h"

void ttl_send_data(uint8_t *buf, uint16_t len);
void ttl_init(void);
#endif
