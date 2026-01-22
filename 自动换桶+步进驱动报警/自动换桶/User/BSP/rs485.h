#ifndef __RS485_H__
#define __RS485_H__

#include "gd32f30x.h"

#define     A_UART              USART0
#define     A_UART_PORT         GPIOA
#define     A_UART_PORT_RCU     RCU_GPIOA
#define     A_UART_RCU          RCU_USART0
#define     A_UART_IRQn         USART0_IRQn
#define     A_UART_RX_PIN       GPIO_PIN_10
#define     A_UART_TX_PIN       GPIO_PIN_9
#define     A_UART_CTL_PORT     GPIOA
#define     A_UART_CTL_PIN      GPIO_PIN_11
#define     A_UART_CTL_PORT_RCU RCU_GPIOA
#define     A_UART_DMA          DMA0
#define     A_UART_DMA_RCU      RCU_DMA0
#define     A_UART_DMA_CH       DMA_CH4
#define     A_DATA_ADDR         USART0_DATA_ADDRESS
#define     A_BAUDRATE 		9600U//115200U//motify

#define     USART0_DATA_ADDRESS ((uint32_t)&USART_DATA(USART0))

typedef struct {
	uint8_t     irq;

    uint32_t    uart_rx_gpio;
    uint32_t    uart_tx_gpio;
    uint32_t    uart_rx_pin;
    uint32_t    uart_tx_pin;
    uint32_t    uart_ctl_pin;
    uint32_t    uart_ctl_gpio;

    uint32_t    uart_no;
    uint32_t    dma_no;
    
    rcu_periph_enum     uart_rx_gpio_rcu;
    rcu_periph_enum     uart_tx_gpio_rcu;
    rcu_periph_enum     uart_ctl_gpio_rcu;
    rcu_periph_enum     uart_rcu;
    dma_channel_enum    dma_ch;
    rcu_periph_enum     rcu_dma;
	usart_interrupt_enum uart_interrupt;
} rs485_hw_info_t;

typedef struct {
	rs485_hw_info_t 	hw_info;
	uint8_t 			buffer[50];
	uint8_t 			size;
} rs485_t;

void rs485_init(void);
int rs485_write(rs485_t *const u, const uint8_t * const buffer, uint8_t size);

#endif
