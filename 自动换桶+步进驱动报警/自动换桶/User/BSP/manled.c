#include "manled.h"

static void man_led_gpio_init(void)
{
	rcu_periph_clock_enable(MAN_LED_RED_GPIO_RCU);
	rcu_periph_clock_enable(MAN_LED_GREEN_GPIO_RCU);
	rcu_periph_clock_enable(MAN_LED_BUZZER_GPIO_RCU);
	
	gpio_bit_reset(MAN_LED_RED_GPIO, MAN_LED_RED_PIN);
	gpio_bit_reset(MAN_LED_GREEN_GPIO, MAN_LED_GREEN_PIN);
	gpio_bit_reset(MAN_LED_BUZZER_GPIO, MAN_LED_BUZZER_PIN);
	
	gpio_init(MAN_LED_RED_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, MAN_LED_RED_PIN);
	gpio_init(MAN_LED_BUZZER_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, MAN_LED_BUZZER_PIN);
	gpio_init(MAN_LED_GREEN_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, MAN_LED_GREEN_PIN);
}

void man_led_ctl(man_led_enum_t led_, SWITCH_STATE state)
{
	switch(led_)
	{
		case MAN_LED_RED :
			if(state == ON)
				gpio_bit_set(MAN_LED_RED_GPIO, MAN_LED_RED_PIN);
			else
				gpio_bit_reset(MAN_LED_RED_GPIO, MAN_LED_RED_PIN);
			break;
		case MAN_LED_GREEN :
			if(state == ON)
				gpio_bit_set(MAN_LED_GREEN_GPIO, MAN_LED_GREEN_PIN);
			else
				gpio_bit_reset(MAN_LED_GREEN_GPIO, MAN_LED_GREEN_PIN);
			break;
		case MAN_LED_BUZZER :
			if(state == ON)
				gpio_bit_set(MAN_LED_BUZZER_GPIO, MAN_LED_BUZZER_PIN);
			else
				gpio_bit_reset(MAN_LED_BUZZER_GPIO, MAN_LED_BUZZER_PIN);
			break;
		default :
			break;
	}
}

void man_led_init(void)
{
	man_led_gpio_init();
	
	man_led_ctl(MAN_LED_RED, ON);
	man_led_ctl(MAN_LED_GREEN, OFF);
	man_led_ctl(MAN_LED_BUZZER, OFF);
}
