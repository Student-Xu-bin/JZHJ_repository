#ifndef __FULL_LIGHT_H__
#define __FULL_LIGHT_H__

#include "gd32f30x.h"

#define FULL_LIGHT_LOW 		1
#define FULL_LIGHT_HIGH 	0

#define WS2812B_1_DATA_PORT     	GPIOA
#define WS2812B_1_DATA_PIN      	GPIO_PIN_1
#define WS2812B_1_DATA_PORT_RCU 	RCU_GPIOA

#define WS2812B_2_DATA_PORT     	GPIOA
#define WS2812B_2_DATA_PIN      	GPIO_PIN_5
#define WS2812B_2_DATA_PORT_RCU 	RCU_GPIOA

#define WS2812B_3_DATA_PORT    	 	GPIOA
#define WS2812B_3_DATA_PIN      	GPIO_PIN_7
#define WS2812B_3_DATA_PORT_RCU 	RCU_GPIOA

#define WS2812B_4_DATA_PORT     	GPIOC
#define WS2812B_4_DATA_PIN      	GPIO_PIN_5
#define WS2812B_4_DATA_PORT_RCU 	RCU_GPIOC

#define WS2812B_5_DATA_PORT     	GPIOB
#define WS2812B_5_DATA_PIN      	GPIO_PIN_1
#define WS2812B_5_DATA_PORT_RCU 	RCU_GPIOB

#define WS2812B_6_DATA_PORT    		GPIOE
#define WS2812B_6_DATA_PIN      	GPIO_PIN_8
#define WS2812B_6_DATA_PORT_RCU 	RCU_GPIOE

struct full_light_hw_info_t {
	uint32_t 		gpio;
	uint32_t 		pin;
	rcu_periph_enum gpio_rcu;
};

typedef struct ws2812b_ {
	struct full_light_hw_info_t 	hw_info;
	uint8_t  numbers; 	// µÆµÄ¸öÊý
} full_light_t;

int full_light_init(full_light_t *);
void full_light_ctl(full_light_t *const full_light, uint8_t green, uint8_t red, uint8_t blue);

#endif
