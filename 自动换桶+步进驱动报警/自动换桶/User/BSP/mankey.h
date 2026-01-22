#ifndef __MAN_KEY_H__
#define __MAN_KEY_H__

#include "gd32f30x.h"
#include "defines.h"

#define MAN_KEY_RED_GPIO 			GPIOE
#define MAN_KEY_RED_PIN 			GPIO_PIN_15
#define MAN_KEY_RED_GPIO_RCU 		RCU_GPIOE

#define MAN_KEY_GREEN_GPIO 			GPIOE
#define MAN_KEY_GREEN_PIN 			GPIO_PIN_14
#define MAN_KEY_GREEN_GPIO_RCU 		RCU_GPIOE

typedef enum {
	MAN_KEY_RED = 0x87, MAN_KEY_GREEN = 0x97
} man_key_enum_t;

void man_key_init(void);
FlagStatus man_key_detect(man_key_enum_t wk);

#endif
