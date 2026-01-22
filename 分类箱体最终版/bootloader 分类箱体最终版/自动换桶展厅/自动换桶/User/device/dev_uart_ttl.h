#ifndef __DEV_UART_TTL_H__
#define __DEV_UART_TTL_H__

#include "config.h"

#define KEY_485_USART         UART3
#define KEY_485_GPIO          GPIOC
#define KEY_485_GPIO_RCU      RCU_GPIOC
#define KEY_485_RCU_USART 	  RCU_UART3
#define KEY_485_RX_PIN        GPIO_PIN_11
#define KEY_485_TX_PIN        GPIO_PIN_10
#define KEY_485_BAUDRATE      9600U

#endif /* __DEV_UART_TTL_H__ */
