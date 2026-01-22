#ifndef __MANLED_H__
#define __MANLED_H__

#include "gd32f30x.h"
#include "defines.h"

#define MAN_LED_RED_GPIO 			GPIOB
#define MAN_LED_RED_PIN 			GPIO_PIN_11
#define MAN_LED_RED_GPIO_RCU 		RCU_GPIOB

#define MAN_LED_GREEN_GPIO 			GPIOB
#define MAN_LED_GREEN_PIN 			GPIO_PIN_10
#define MAN_LED_GREEN_GPIO_RCU 		RCU_GPIOB

#define MAN_LED_BUZZER_GPIO 		GPIOB
#define MAN_LED_BUZZER_PIN 			GPIO_PIN_2
#define MAN_LED_BUZZER_GPIO_RCU 	RCU_GPIOB

typedef enum {
	MAN_LED_RED, MAN_LED_GREEN, MAN_LED_BUZZER
} man_led_enum_t;

void man_led_ctl(man_led_enum_t led_, SWITCH_STATE state);
void man_led_init(void);

#endif
