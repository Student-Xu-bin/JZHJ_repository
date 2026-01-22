#ifndef __DEV_USER_LED_H__
#define __DEV_USER_LED_H__ 

#include "gd32f30x.h"

#define USER_LED_RCU_PORT      RCU_GPIOC
#define USER_LED_PORT          GPIOC
#define USER_LED_PIN           GPIO_PIN_3

#define USER_LED_ON()  (gpio_bit_set(GPIOC, GPIO_PIN_3))
#define USER_LED_OFF() (gpio_bit_reset(GPIOC, GPIO_PIN_3))

void user_led_gpio_init(void);

#endif
