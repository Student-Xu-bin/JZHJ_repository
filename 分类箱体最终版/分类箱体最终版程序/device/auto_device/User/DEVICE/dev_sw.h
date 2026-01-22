#ifndef __DEV_SW_H__
#define __DEV_SW_H__

#include "gd32f30x.h"

#define SW_HUMAN_RCU_PORT      RCU_GPIOC
#define SW_HUMAN_PORT          GPIOC
#define SW_HUMAN_PIN           GPIO_PIN_1

#define SW_LIGHT_RCU_PORT      RCU_GPIOC
#define SW_LIGHT_PORT          GPIOC
#define SW_LIGHT_PIN           GPIO_PIN_0

#define SW_READ_HUMAN() 	  (gpio_input_bit_get(GPIOC, GPIO_PIN_1))
#define SW_READ_LIGHT() 	  (gpio_input_bit_get(GPIOC, GPIO_PIN_0))

void sw_gpio_init(void);

#endif
