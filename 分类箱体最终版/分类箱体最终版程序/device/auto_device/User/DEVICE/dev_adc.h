#ifndef __DEV_ADC_H__
#define __DEV_ADC_H__ 

#include "gd32f30x.h"

#if 0
#define ADC_1_RCU_GPIO		RCU_GPIOB
#define ADC_1_GPIO			GPIOB
#define ADC_1_PIN			GPIO_PIN_0

#define ADC_2_RCU_GPIO		RCU_GPIOB
#define ADC_2_GPIO			GPIOB
#define ADC_2_PIN			GPIO_PIN_1

#define ADC_3_RCU_GPIO		RCU_GPIOA
#define ADC_3_GPIO			GPIOA
#define ADC_3_PIN			GPIO_PIN_0

#define ADC_4_RCU_GPIO		RCU_GPIOA
#define ADC_4_GPIO			GPIOA
#define ADC_4_PIN			GPIO_PIN_1

#define ADC_5_RCU_GPIO		RCU_GPIOC
#define ADC_5_GPIO			GPIOC
#define ADC_5_PIN			GPIO_PIN_5

#define ADC_6_RCU_GPIO		RCU_GPIOC
#define ADC_6_GPIO			GPIOC
#define ADC_6_PIN			GPIO_PIN_4
#endif

#define ADC_1_RCU_GPIO		RCU_GPIOA
#define ADC_1_GPIO			GPIOA
#define ADC_1_PIN			GPIO_PIN_1

#define ADC_2_RCU_GPIO		RCU_GPIOA
#define ADC_2_GPIO			GPIOA
#define ADC_2_PIN			GPIO_PIN_0

#define ADC_3_RCU_GPIO		RCU_GPIOC
#define ADC_3_GPIO			GPIOC
#define ADC_3_PIN			GPIO_PIN_4

#define ADC_4_RCU_GPIO		RCU_GPIOC
#define ADC_4_GPIO			GPIOC
#define ADC_4_PIN			GPIO_PIN_5

#define ADC_5_RCU_GPIO		RCU_GPIOB
#define ADC_5_GPIO			GPIOB
#define ADC_5_PIN			GPIO_PIN_0

#define ADC_6_RCU_GPIO		RCU_GPIOB
#define ADC_6_GPIO			GPIOB
#define ADC_6_PIN			GPIO_PIN_1

void dev_adc_gpio_init(void);

typedef struct
{
	rcu_periph_enum rcu_gpio;
    uint32_t        gpio;
    uint32_t        pin;
}dev_adc_t;

#endif
