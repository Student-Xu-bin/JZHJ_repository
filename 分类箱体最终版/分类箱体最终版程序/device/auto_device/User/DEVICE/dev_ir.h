#ifndef __DEV_IR_H__
#define __DEV_IR_H__ 

#include "gd32f30x.h"

#define IR_1_GPIO_PIN               GPIO_PIN_15
#define IR_1_GPIO_PORT	            GPIOA
#define IR_1_GPIO_RCU_PORT          RCU_GPIOA

// #define IR_2_GPIO_PIN               GPIO_PIN_15
// #define IR_2_GPIO_PORT	            GPIOA
// #define IR_2_GPIO_RCU_PORT          RCU_GPIOA

// #define IR_3_GPIO_PIN            GPIO_PIN_2
// #define IR_3_GPIO_PORT	        GPIOA
// #define IR_3_GPIO_RCU_PORT       RCU_GPIOA

#define IR_4_GPIO_PIN               GPIO_PIN_10
#define IR_4_GPIO_PORT	            GPIOB
#define IR_4_GPIO_RCU_PORT          RCU_GPIOB

#define IR_5_GPIO_PIN               GPIO_PIN_7
#define IR_5_GPIO_PORT	            GPIOA
#define IR_5_GPIO_RCU_PORT          RCU_GPIOA

// #define IR_6_GPIO_PIN        GPIO_PIN_3
// #define IR_6_GPIO_PORT	        GPIOB
// #define IR_6_GPIO_RCU_PORT       RCU_GPIOB

#define IR_1_DET() (gpio_input_bit_get(GPIOA, GPIO_PIN_15))
#define IR_4_DET() (gpio_input_bit_get(GPIOB, GPIO_PIN_10))
#define IR_5_DET() (gpio_input_bit_get(GPIOA, GPIO_PIN_7))

void dev_ir_gpio_init(void);

#endif

