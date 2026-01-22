#ifndef __SOLENOID_VALVE_H__
#define __SOLENOID_VALVE_H__

#include "gd32f30x.h"

#define 	SOLENOID_VALVE_CTL_1_PORT_1			GPIOB
#define 	SOLENOID_VALVE_CTL_1_PIN_1			GPIO_PIN_5
#define 	SOLENOID_VALVE_CTL_1_PORT_RCU_1		RCU_GPIOB
#define 	SOLENOID_VALVE_CTL_2_PORT_1			GPIOB
#define 	SOLENOID_VALVE_CTL_2_PIN_1			GPIO_PIN_4
#define 	SOLENOID_VALVE_CTL_2_PORT_RCU_1		RCU_GPIOB
#define 	SOLENOID_VALVE_CTL_3_PORT_1			GPIOB
#define 	SOLENOID_VALVE_CTL_3_PIN_1			GPIO_PIN_3
#define 	SOLENOID_VALVE_CTL_3_PORT_RCU_1		RCU_GPIOB
#define 	SOLENOID_VALVE_CTL_4_PORT_1			GPIOD
#define 	SOLENOID_VALVE_CTL_4_PIN_1			GPIO_PIN_7
#define 	SOLENOID_VALVE_CTL_4_PORT_RCU_1		RCU_GPIOD

#define 	SOLENOID_VALVE_CTL_1_PORT_2			GPIOD
#define 	SOLENOID_VALVE_CTL_1_PIN_2			GPIO_PIN_6
#define 	SOLENOID_VALVE_CTL_1_PORT_RCU_2		RCU_GPIOD
#define 	SOLENOID_VALVE_CTL_2_PORT_2			GPIOD
#define 	SOLENOID_VALVE_CTL_2_PIN_2			GPIO_PIN_5
#define 	SOLENOID_VALVE_CTL_2_PORT_RCU_2		RCU_GPIOD
#define 	SOLENOID_VALVE_CTL_3_PORT_2			GPIOD
#define 	SOLENOID_VALVE_CTL_3_PIN_2			GPIO_PIN_4
#define 	SOLENOID_VALVE_CTL_3_PORT_RCU_2		RCU_GPIOD
#define 	SOLENOID_VALVE_CTL_4_PORT_2			GPIOD
#define 	SOLENOID_VALVE_CTL_4_PIN_2			GPIO_PIN_3
#define 	SOLENOID_VALVE_CTL_4_PORT_RCU_2		RCU_GPIOD

#define 	SOLENOID_VALVE_CTL_1_PORT_3			GPIOD
#define 	SOLENOID_VALVE_CTL_1_PIN_3 			GPIO_PIN_2
#define 	SOLENOID_VALVE_CTL_1_PORT_RCU_3		RCU_GPIOD
#define 	SOLENOID_VALVE_CTL_2_PORT_3			GPIOD
#define 	SOLENOID_VALVE_CTL_2_PIN_3			GPIO_PIN_1
#define 	SOLENOID_VALVE_CTL_2_PORT_RCU_3		RCU_GPIOD
#define 	SOLENOID_VALVE_CTL_3_PORT_3			GPIOD
#define 	SOLENOID_VALVE_CTL_3_PIN_3			GPIO_PIN_0
#define 	SOLENOID_VALVE_CTL_3_PORT_RCU_3		RCU_GPIOD
#define 	SOLENOID_VALVE_CTL_4_PORT_3			GPIOC
#define 	SOLENOID_VALVE_CTL_4_PIN_3			GPIO_PIN_12
#define 	SOLENOID_VALVE_CTL_4_PORT_RCU_3		RCU_GPIOC

#define 	SV_ON 		SET
#define 	SV_OFF 		RESET

struct solenoid_valve_hw_info_t {
	uint32_t 			gpio;
	uint32_t 			pin;
	rcu_periph_enum		gpio_rcu;
};

typedef struct solenoid_valve_ {
	struct solenoid_valve_hw_info_t 	hw_info;
} solenoid_valve_t;

int solenoid_valve_init(solenoid_valve_t *sv);
void solenoid_valve_ctl(solenoid_valve_t *sv, FlagStatus s);
FlagStatus solenoid_valve_state(solenoid_valve_t *sv);

#endif
