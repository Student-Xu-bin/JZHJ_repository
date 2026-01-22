#ifndef __DEV_MOTOR_H
#define __DEV_MOTOR_H

#include "gd32f30x.h"

#define MOTOR1_CTL_PORT1 			GPIOB
#define MOTOR1_CTL_PIN1             GPIO_PIN_5
#define MOTOR1_CTL_PORT2 			GPIOB
#define MOTOR1_CTL_PIN2             GPIO_PIN_6
#define MOTOR1_CTL1_PORT_RCU        RCU_GPIOB
#define MOTOR1_CTL2_PORT_RCU        RCU_GPIOB

#define MOTOR3_CTL_PORT1 			GPIOA
#define MOTOR3_CTL_PIN1             GPIO_PIN_11
#define MOTOR3_CTL_PORT2 			GPIOA
#define MOTOR3_CTL_PIN2             GPIO_PIN_12
#define MOTOR3_CTL1_PORT_RCU        RCU_GPIOA
#define MOTOR3_CTL2_PORT_RCU        RCU_GPIOA

#define MOTOR2_CTL_PORT1 			GPIOB
#define MOTOR2_CTL_PIN1             GPIO_PIN_3
#define MOTOR2_CTL_PORT2 			GPIOB
#define MOTOR2_CTL_PIN2             GPIO_PIN_4
#define MOTOR2_CTL1_PORT_RCU        RCU_GPIOB
#define MOTOR2_CTL2_PORT_RCU        RCU_GPIOB

#if 0
#define MOTOR4_CTL_PORT1 			GPIOC
#define MOTOR4_CTL_PIN1             GPIO_PIN_7
#define MOTOR4_CTL_PORT2 			GPIOC
#define MOTOR4_CTL_PIN2             GPIO_PIN_8
#define MOTOR4_CTL1_PORT_RCU        RCU_GPIOC
#define MOTOR4_CTL2_PORT_RCU        RCU_GPIOC

#define MOTOR5_CTL_PORT1 			GPIOB
#define MOTOR5_CTL_PIN1             GPIO_PIN_15
#define MOTOR5_CTL_PORT2 			GPIOC
#define MOTOR5_CTL_PIN2             GPIO_PIN_6
#define MOTOR5_CTL1_PORT_RCU        RCU_GPIOB
#define MOTOR5_CTL2_PORT_RCU        RCU_GPIOC

#define MOTOR6_CTL_PORT1 			GPIOC
#define MOTOR6_CTL_PIN1             GPIO_PIN_9
#define MOTOR6_CTL_PORT2 			GPIOA
#define MOTOR6_CTL_PIN2             GPIO_PIN_8
#define MOTOR6_CTL1_PORT_RCU        RCU_GPIOC
#define MOTOR6_CTL2_PORT_RCU        RCU_GPIOA
#endif

#define MOTOR4_CTL_PORT1 			GPIOC
#define MOTOR4_CTL_PIN1             GPIO_PIN_9
#define MOTOR4_CTL_PORT2 			GPIOA
#define MOTOR4_CTL_PIN2             GPIO_PIN_8
#define MOTOR4_CTL1_PORT_RCU        RCU_GPIOC
#define MOTOR4_CTL2_PORT_RCU        RCU_GPIOA

#define MOTOR5_CTL_PORT1 			GPIOC
#define MOTOR5_CTL_PIN1             GPIO_PIN_7
#define MOTOR5_CTL_PORT2 			GPIOC
#define MOTOR5_CTL_PIN2             GPIO_PIN_8
#define MOTOR5_CTL1_PORT_RCU        RCU_GPIOC
#define MOTOR5_CTL2_PORT_RCU        RCU_GPIOC

#define MOTOR6_CTL_PORT1 			GPIOB
#define MOTOR6_CTL_PIN1             GPIO_PIN_15
#define MOTOR6_CTL_PORT2 			GPIOC
#define MOTOR6_CTL_PIN2             GPIO_PIN_6
#define MOTOR6_CTL1_PORT_RCU        RCU_GPIOB
#define MOTOR6_CTL2_PORT_RCU        RCU_GPIOC


#define MOTOR_OPEN  1
#define MOTOR_CLOSE 2
#define MOTOR_STOP  0

typedef struct{
	rcu_periph_enum rcu_port_1;
	rcu_periph_enum rcu_port_2;
	uint32_t 		port_1;
	uint32_t 		port_2;
	uint32_t 		pin_1;
	uint32_t 		pin_2;
	uint8_t 		state;
	uint16_t        zero_current;
	uint16_t        limit_currnet;
	uint8_t  		no;
	uint8_t         close_state;
	uint8_t         open_state;
	uint8_t 		channel_init_flag;
	uint8_t 		error_flag;
}motor_t;



extern motor_t motor[6];
void motor_gpio_init(motor_t *p);

#endif
