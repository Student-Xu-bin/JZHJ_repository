#include "mankey.h"

static void man_key_gpio_init(void)
{
	rcu_periph_clock_enable(MAN_KEY_RED_GPIO_RCU);
	rcu_periph_clock_enable(MAN_KEY_GREEN_GPIO_RCU);
	
	gpio_init(MAN_KEY_RED_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, MAN_KEY_RED_PIN);
	gpio_init(MAN_KEY_GREEN_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, MAN_KEY_GREEN_PIN);
}

void man_key_init(void)
{
	man_key_gpio_init();
}

FlagStatus man_key_detect(man_key_enum_t wk)
{
	uint32_t gpio, pin;
	
	if(wk == MAN_KEY_RED)
	{
		gpio = MAN_KEY_RED_GPIO;
		pin = MAN_KEY_RED_PIN;
	}
	else if(wk == MAN_KEY_GREEN)
	{
		gpio = MAN_KEY_GREEN_GPIO;
		pin = MAN_KEY_GREEN_PIN;
	}
	
	if(gpio_input_bit_get(gpio, pin) == SET)
		return RESET;
	
	return SET;
}
