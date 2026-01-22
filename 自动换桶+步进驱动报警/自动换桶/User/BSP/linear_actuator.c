#include "linear_actuator.h"

void linear_actuator_ctl(linear_actuator_t *la, enum linear_actuator_state_enum_t s)
{
	switch(s)
	{
	case LA_STOP :
		gpio_bit_reset(la->hw_info.ctl1_gpio, la->hw_info.ctl1_pin);
		gpio_bit_reset(la->hw_info.ctl2_gpio, la->hw_info.ctl2_pin);
		break;
	case LA_CLOSE :
		gpio_bit_reset(la->hw_info.ctl2_gpio, la->hw_info.ctl2_pin);
		gpio_bit_set(la->hw_info.ctl1_gpio, la->hw_info.ctl1_pin);
		break;
	case LA_OPEN :
		gpio_bit_reset(la->hw_info.ctl1_gpio, la->hw_info.ctl1_pin);
		gpio_bit_set(la->hw_info.ctl2_gpio, la->hw_info.ctl2_pin);
		break;
	}
	
	la->state = s;
}

int linear_actuator_init(linear_actuator_t * la)
{
	if(!la)
		return -1;
	
	rcu_periph_clock_enable(la->hw_info.ctl1_gpio_rcu);
	rcu_periph_clock_enable(la->hw_info.ctl2_gpio_rcu);
	
	gpio_bit_reset(la->hw_info.ctl1_gpio, la->hw_info.ctl1_pin);
	gpio_bit_reset(la->hw_info.ctl2_gpio, la->hw_info.ctl2_pin);
	la->state = LA_STOP;
	
	gpio_init(la->hw_info.ctl1_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, la->hw_info.ctl1_pin);
	gpio_init(la->hw_info.ctl2_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, la->hw_info.ctl2_pin);
	
	return 0;
}
