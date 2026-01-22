#ifndef __GARBAGE_CONFIG_H
#define __GARBAGE_CONFIG_H

#include "garbage_usart.h"
#include <memory>
#include <vector>
#include "garbage_pitch.h"
extern "C"
{
#include "74hc595.h"
#include "delay.h"
#include "gd32f30x.h"
#include "modbus_app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "modbus_app.h"
}

class garbage
{
	public:
		using pit_t = std::unique_ptr<garbage_pitch>;
		static garbage *creat_garbage();
	
		std::unique_ptr<garbage_usart> usart;
		std::vector<pit_t> pitches;
		void pitch_init();
	
		void adc_init();
		void adc_get_zero();
	
		void usart_init(uint32_t usart_no, uint32_t baudrate);
		void usart_send_data(uint32_t usart_no, uint8_t *buf, uint8_t len);
		
		enum class bsp_led_status_t : uint8_t
		{
			OFF = 0x00,
			ON 	= 0x01,
		};
		void bsp_led_init();
		void bsp_led_ctl(bsp_led_status_t bsp_led_statut);
		
		void garbage_initialize();
		
	private:
		garbage();
		static garbage *single_garbage;
};


#define 	DOOR_OPEN_START 				(1 << 0)
#define 	DOOR_CLOSE_START 				(1 << 1)
#define     DOOR_GET_CURRENT				(1 << 2)
#define 	DOOR_ALL_EVENTS					(0x07)





/*LED*/
#if 1
#define USER_LED_GPIO         		 	GPIOA
#define USER_LED_GPIO_RCU        		RCU_GPIOA
#define USER_LED_PIN            		GPIO_PIN_15
#endif
/*WS2812B & full_led*/
#if 1
#define FULL_LED_1_GPIO_RCU 		RCU_GPIOC
#define FULL_LED_1_GPIO 			GPIOC
#define FULL_LED_1_PIN 				GPIO_PIN_0

#define FULL_LED_2_GPIO_RCU 		RCU_GPIOB
#define FULL_LED_2_GPIO 			GPIOB
#define FULL_LED_2_PIN 				GPIO_PIN_5

#define FULL_LED_3_GPIO_RCU 		RCU_GPIOB
#define FULL_LED_3_GPIO 			GPIOB
#define FULL_LED_3_PIN 				GPIO_PIN_14

#define FULL_LED_4_GPIO_RCU 		RCU_GPIOB
#define FULL_LED_4_GPIO 			GPIOB
#define FULL_LED_4_PIN 				GPIO_PIN_11

#define FULL_LED_5_GPIO_RCU 		RCU_GPIOA
#define FULL_LED_5_GPIO 			GPIOA
#define FULL_LED_5_PIN 				GPIO_PIN_7

#define FULL_LED_6_GPIO_RCU 		RCU_GPIOB
#define FULL_LED_6_GPIO 			GPIOB
#define FULL_LED_6_PIN 				GPIO_PIN_9
#endif
/*KEY*/
#if 1
#define KEY_1_GPIO						GPIOC
#define KEY_1_GPIO_RCU					RCU_GPIOC
#define KEY_1_PIN						GPIO_PIN_14

#define KEY_2_GPIO						GPIOB
#define KEY_2_GPIO_RCU					RCU_GPIOB
#define KEY_2_PIN						GPIO_PIN_4

#define KEY_3_GPIO						GPIOC
#define KEY_3_GPIO_RCU					RCU_GPIOC
#define KEY_3_PIN						GPIO_PIN_1

#define KEY_4_GPIO						GPIOB
#define KEY_4_GPIO_RCU					RCU_GPIOB
#define KEY_4_PIN						GPIO_PIN_12

#define KEY_5_GPIO          				GPIOA
#define KEY_5_GPIO_RCU     	  			RCU_GPIOA
#define KEY_5_PIN         	  			GPIO_PIN_6

#define KEY_6_GPIO						GPIOC
#define KEY_6_GPIO_RCU					RCU_GPIOC
#define KEY_6_PIN						GPIO_PIN_13
#endif
/*MOTOR*/
#if 1
#define MOTOR1_CTL_PORT1 			GPIOC
#define MOTOR1_CTL_PIN1             GPIO_PIN_12
#define MOTOR1_CTL_PORT2 			GPIOD
#define MOTOR1_CTL_PIN2             GPIO_PIN_2
#define MOTOR1_CTL1_PORT_RCU        RCU_GPIOC
#define MOTOR1_CTL2_PORT_RCU        RCU_GPIOD

#define MOTOR2_CTL_PORT1 			GPIOB
#define MOTOR2_CTL_PIN1             GPIO_PIN_6
#define MOTOR2_CTL_PORT2 			GPIOB
#define MOTOR2_CTL_PIN2             GPIO_PIN_8
#define MOTOR2_CTL1_PORT_RCU        RCU_GPIOB
#define MOTOR2_CTL2_PORT_RCU        RCU_GPIOB

#define MOTOR3_CTL_PORT1 			GPIOA
#define MOTOR3_CTL_PIN1             GPIO_PIN_12
#define MOTOR3_CTL_PORT2 			GPIOA
#define MOTOR3_CTL_PIN2             GPIO_PIN_11
#define MOTOR3_CTL1_PORT_RCU        RCU_GPIOA
#define MOTOR3_CTL2_PORT_RCU        RCU_GPIOA

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
#endif



#endif
