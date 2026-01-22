#ifndef __74HC165_H__
#define __74HC165_H__ 

#include "gd32f30x.h"

#define     HC165_DATA_GPIO     GPIOB
#define     HC165_DATA_PIN      GPIO_PIN_14
#define     HC165_DATA_RCU      RCU_GPIOB

#define     HC165_CP_GPIO       GPIOB//时钟
#define     HC165_CP_PIN        GPIO_PIN_13
#define     HC165_CP_RCU        RCU_GPIOB

#define     HC165_PL_GPIO       GPIOB//输出使能
#define     HC165_PL_PIN        GPIO_PIN_12
#define     HC165_PL_RCU        RCU_GPIOB


void _74hc165_gpio_init(void);
uint16_t _74hc165_read_data(void);


#endif

