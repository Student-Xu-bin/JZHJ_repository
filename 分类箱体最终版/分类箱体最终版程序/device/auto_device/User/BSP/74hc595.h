#ifndef __74HC595_H__
#define __74HC595_H__

#include "gd32f30x.h"

#define     HC959_DS_GPIO       GPIOB
#define     HC595_DS_PIN        GPIO_PIN_9
#define     HC595_DS_RCU        RCU_GPIOB

#define     HC595_STCP_GPIO     GPIOC
#define     HC959_STCP_PIN      GPIO_PIN_12
#define     HC595_STCP_RCU      RCU_GPIOC

#define     HC595_SHCP_GPIO     GPIOD
#define     HC595_SHCP_PIN      GPIO_PIN_2
#define     HC595_SHCP_RCU      RCU_GPIOD

enum channel_enum_t {CHANNEL_1 = 0, CHANNEL_2, CHANNEL_3, CHANNEL_4, CHANNEL_5, CHANNEL_6, RS485_SLAVE, RS485_MASTER};
enum function_enum_t {RED_LED, RS485_RECEIVE, RS485_SEND};
typedef enum {OFF = 0, ON}state_enum_t;

union _74hc595_data_bit {
	struct _bit {
		uint8_t  rs485_slave_ctl  : 1;
		uint8_t  rs485_master_ctl : 1;
		uint8_t  en_red_led_6 : 1;
		uint8_t  en_red_led_5 : 1;
		uint8_t  en_red_led_4 : 1;
		uint8_t  en_red_led_1 : 1;
		uint8_t  en_red_led_2 : 1;
		uint8_t  en_red_led_3 : 1;
	} bit;
	uint8_t data[1];
};

typedef struct _74hc595
{
    uint32_t ds_gpio;
    uint32_t stcp_gpio;
    uint32_t shcp_gpio;
    uint32_t ds_pin;
    uint32_t stcp_pin;//shift register clock input
    uint32_t shcp_pin;//storage register clock input

    rcu_periph_enum ds_gpio_rcu;
    rcu_periph_enum stcp_gpio_rcu;
    rcu_periph_enum shcp_gpio_rcu;
} _74hc595_t;

void _74hc595_init(void);
void _74hc595_send_data_buffer_clear(union _74hc595_data_bit *data);
	#if 0
void _74hc595_ctl(union _74hc595_data_bit *data);
	#else
void _74hc595_ctl(enum channel_enum_t ch, enum function_enum_t fun, state_enum_t state, uint8_t set_);
	#endif

#endif
