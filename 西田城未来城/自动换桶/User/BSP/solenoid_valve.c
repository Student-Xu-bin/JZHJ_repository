#include "solenoid_valve.h"

void solenoid_valve_ctl(solenoid_valve_t *sv, FlagStatus s)
{
	(s == SV_ON) ? (gpio_bit_set(sv->hw_info.gpio, sv->hw_info.pin)) :
			       (gpio_bit_reset(sv->hw_info.gpio, sv->hw_info.pin));
}

int solenoid_valve_init(solenoid_valve_t *sv)
{
	if(!sv)
		return -1;
	
	if(sv->hw_info.gpio == GPIOB && sv->hw_info.pin == GPIO_PIN_4)
	{
		rcu_periph_clock_enable(RCU_AF);
		gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	}
	
	rcu_periph_clock_enable(sv->hw_info.gpio_rcu);
	
	gpio_bit_reset(sv->hw_info.gpio, sv->hw_info.pin);
	gpio_init(sv->hw_info.gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, sv->hw_info.pin);
	
	return 0;
}

FlagStatus solenoid_valve_state(solenoid_valve_t *sv)
{
	return gpio_input_bit_get(sv->hw_info.gpio, sv->hw_info.pin);
}
