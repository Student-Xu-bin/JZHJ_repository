#ifndef __DEV_HUMAN_H__
#define __DEV_HUMAN_H__ 

#include "gd32f30x.h"

#define HUMAN_1_GPIO_RCU_PORT       RCU_GPIOC
#define HUMAN_1_GPIO_PORT	        GPIOC
#define HUMAN_1_GPIO_PIN            GPIO_PIN_13

// #define HUMAN_2_GPIO_RCU_PORT       RCU_GPIOC
// #define HUMAN_2_GPIO_PORT			GPIOC
// #define HUMAN_2_GPIO_PIN            GPIO_PIN_14

// #define HUMAN_3_GPIO_RCU_PORT     RCU_GPIOC
// #define HUMAN_3_GPIO_PORT			GPIOC
// #define HUMAN_3_GPIO_PIN          GPIO_PIN_15

// #define HUMAN_4_GPIO_RCU_PORT     RCU_GPIOC
// #define HUMAN_4_GPIO_PORT			GPIOC
// #define HUMAN_4_GPIO_PIN          GPIO_PIN_0

#define HUMAN_5_GPIO_RCU_PORT       RCU_GPIOA
#define HUMAN_5_GPIO_PORT			GPIOA
#define HUMAN_5_GPIO_PIN            GPIO_PIN_4

// #define HUMAN_6_GPIO_RCU_PORT     RCU_GPIOC
// #define HUMAN_6_GPIO_PORT			GPIOC
// #define HUMAN_6_GPIO_PIN          GPIO_PIN_2

#define HUMAN_1_DET() (gpio_input_bit_get(GPIOC, GPIO_PIN_13))
#define HUMAN_5_DET() (gpio_input_bit_get(GPIOA, GPIO_PIN_4))

void dev_human_gpio_init(void);

#endif

