#ifndef __74HC595_H__
#define __74HC595_H__

#include "gd32f30x.h"
#include "defines.h"

#define     HC959_DS_GPIO       GPIOE
#define     HC595_DS_PIN        GPIO_PIN_11
#define     HC595_DS_RCU        RCU_GPIOE

#define     HC595_STCP_GPIO     GPIOE
#define     HC959_STCP_PIN      GPIO_PIN_12
#define     HC595_STCP_RCU      RCU_GPIOE

#define     HC595_SHCP_GPIO     GPIOE
#define     HC595_SHCP_PIN      GPIO_PIN_13
#define     HC595_SHCP_RCU      RCU_GPIOE

enum channel_enum_t {CHANNEL_1 = 0, CHANNEL_2, CHANNEL_3, CHANNEL_4, CHANNEL_5, CHANNEL_6, STEPPER_ALARM, CHANNEL_EMPTY};
enum function_enum_t {CD4051_A0 = 0, CD4051_A1, CD4051_A2, VCC_MOTOR, VCC_12V, VCC_5V, KEY_LED, DOOR_PCB_LED};

union _74hc595_data_bit {
	struct _bit {
		uint8_t  ch6_cd4051_a0 : 1;
		uint8_t  ch6_cd4051_a1 : 1;
		uint8_t  ch6_cd4051_a2 : 1;
		uint8_t                : 5;


		uint8_t  ch3_cd4051_a1 : 1;
		uint8_t  ch3_cd4051_a2 : 1;
		uint8_t  ch4_cd4051_a0 : 1;
		uint8_t  ch4_cd4051_a1 : 1;
		uint8_t  ch4_cd4051_a2 : 1;
		uint8_t  ch5_cd4051_a0 : 1;
		uint8_t  ch5_cd4051_a1 : 1;
		uint8_t  ch5_cd4051_a2 : 1;
		
		uint8_t  ch1_cd4051_a2 : 1;
		uint8_t  ch2_cd4051_a0 : 1;
		uint8_t  ch2_cd4051_a1 : 1;
		uint8_t  ch2_cd4051_a2 : 1;
		uint8_t  stepper_alarm_cd4051_a0 : 1;
		uint8_t  stepper_alarm_cd4051_a1 : 1;
		uint8_t  stepper_alarm_cd4051_a2 : 1;
		uint8_t  ch3_cd4051_a0 : 1;
		
		uint8_t  ch1_cd4051_a0 : 1;
		uint8_t  ch6_motor_vcc_ctl : 1;
		uint8_t  ch5_motor_vcc_ctl : 1;
		uint8_t  ch4_motor_vcc_ctl : 1;
		uint8_t  ch3_motor_vcc_ctl : 1;
		uint8_t  ch2_motor_vcc_ctl : 1;
		uint8_t  ch1_motor_vcc_ctl : 1;	
		uint8_t  ch1_cd4051_a1 : 1;

		uint8_t  ch5_key_led_ctl : 1;
		uint8_t  ch5_door_led_ctl : 1;
		uint8_t  ch5_12v_ctl : 1;
		uint8_t  ch5_5v_ctl : 1;
		uint8_t  ch6_key_led_ctl : 1;
		uint8_t  ch6_door_led_ctl : 1;
		uint8_t  ch6_12v_ctl : 1;
		uint8_t  ch6_5v_ctl : 1;
		
		uint8_t  ch3_key_led_ctl : 1;
		uint8_t  ch3_door_led_ctl : 1;
		uint8_t  ch3_12v_ctl : 1;
		uint8_t  ch3_5v_ctl : 1;
		uint8_t  ch4_key_led_ctl : 1;
		uint8_t  ch4_door_led_ctl : 1;
		uint8_t  ch4_12v_ctl : 1;
		uint8_t  ch4_5v_ctl : 1;

		uint8_t  ch1_key_led_ctl : 1;
		uint8_t  ch1_door_led_ctl : 1;
		uint8_t  ch1_12v_ctl : 1;
		uint8_t  ch1_5v_ctl : 1;
		uint8_t  ch2_key_led_ctl : 1;
		uint8_t  ch2_door_led_ctl : 1;
		uint8_t  ch2_12v_ctl : 1;
		uint8_t  ch2_5v_ctl : 1;
	} bit;
	uint8_t data[7];
};

typedef struct _74hc595
{
    uint32_t ds_gpio;
    uint32_t stcp_gpio;
    uint32_t shcp_gpio;
    uint32_t ds_pin;
    uint32_t stcp_pin;//shift register clock input
    uint32_t shcp_pin;//storage register clock input

    rcu_periph_enum ds_gpio_rcu;
    rcu_periph_enum stcp_gpio_rcu;
    rcu_periph_enum shcp_gpio_rcu;
} _74hc595_t;

void _74hc595_init(void);
void _74hc595_send_data_buffer_clear(union _74hc595_data_bit *data);
	#if 0
void _74hc595_ctl(union _74hc595_data_bit *data);
	#else
void _74hc595_ctl(enum channel_enum_t ch, enum function_enum_t fun, SWITCH_STATE state, uint8_t set_);
	#endif

#endif
