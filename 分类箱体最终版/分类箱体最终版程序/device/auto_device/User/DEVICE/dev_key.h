#ifndef __DEV_KEY_H__
#define __DEV_KEY_H__ 

#include "gd32f30x.h"

#define KEY_1_GPIO_RCU_PORT         RCU_GPIOC
#define KEY_1_GPIO_PORT	            GPIOC
#define KEY_1_GPIO_PIN              GPIO_PIN_14

// #define KEY_2_GPIO_RCU_PORT         RCU_GPIOC
// #define KEY_2_GPIO_PORT	            GPIOC
// #define KEY_2_GPIO_PIN              GPIO_PIN_14

// #define KEY_3_GPIO_RCU_PORT         RCU_GPIOA
// #define KEY_3_GPIO_PORT	            GPIOA
// #define KEY_3_GPIO_PIN              GPIO_PIN_

// #define KEY_4_GPIO_RCU_PORT         RCU_GPIOA
// #define KEY_4_GPIO_PORT	            GPIOA
// #define KEY_4_GPIO_PIN              GPIO_PIN_

#define KEY_5_GPIO_RCU_PORT         RCU_GPIOA
#define KEY_5_GPIO_PORT	            GPIOA
#define KEY_5_GPIO_PIN              GPIO_PIN_5

// #define KEY_6_GPIO_RCU_PORT         RCU_GPIOA
// #define KEY_6_GPIO_PORT	            GPIOA

#define KEY_1_DET() (gpio_input_bit_get(GPIOC, GPIO_PIN_14))
#define KEY_5_DET() (gpio_input_bit_get(GPIOA, GPIO_PIN_5))

void dev_key_gpio_init(void);

#endif


