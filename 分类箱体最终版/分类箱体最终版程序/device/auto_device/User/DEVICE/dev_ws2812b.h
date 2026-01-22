#ifndef __DEV_WS2812B_H__
#define __DEV_WS2812B_H__ 

#include "gd32f30x.h"

#define WS2812B_1_RCU_PORT      RCU_GPIOC
#define WS2812B_1_PORT          GPIOC
#define WS2812B_1_PIN           GPIO_PIN_15

#define WS2812B_2_RCU_PORT      RCU_GPIOB
#define WS2812B_2_PORT          GPIOB
#define WS2812B_2_PIN           GPIO_PIN_8

#define WS2812B_3_RCU_PORT      RCU_GPIOB
#define WS2812B_3_PORT          GPIOB
#define WS2812B_3_PIN           GPIO_PIN_7

#define WS2812B_4_RCU_PORT      RCU_GPIOB
#define WS2812B_4_PORT          GPIOB
#define WS2812B_4_PIN           GPIO_PIN_11

#define WS2812B_5_RCU_PORT      RCU_GPIOA
#define WS2812B_5_PORT          GPIOA
#define WS2812B_5_PIN           GPIO_PIN_6

#define WS2812B_6_RCU_PORT      RCU_GPIOC
#define WS2812B_6_PORT          GPIOC
#define WS2812B_6_PIN           GPIO_PIN_2

typedef struct
{
    rcu_periph_enum rcu_port;
    uint32_t        gpio_port;
    uint32_t        gpio_pin;
} ws2812b_t;

extern ws2812b_t ws2812b[6];

void dev_ws2812b_gpio_init(ws2812b_t *p);

#endif
