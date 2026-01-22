#include "fulllight.h"

static void full_light_delay(uint32_t n)
{
	while(n --);
}

static inline void ws2812b_data_out(full_light_t * const full_light, uint8_t s)
{
	(s == FULL_LIGHT_LOW) ? (gpio_bit_set(full_light->hw_info.gpio, full_light->hw_info.pin)) :
							(gpio_bit_reset(full_light->hw_info.gpio, full_light->hw_info.pin));
}

static void ws2812b_reset(full_light_t *const full_light)
{
    ws2812b_data_out(full_light, FULL_LIGHT_LOW);
    full_light_delay(10);
}

static void ws2812b_send_one(full_light_t * const full_light)
{
	ws2812b_data_out(full_light, FULL_LIGHT_HIGH);
	full_light_delay(14);
	
	ws2812b_data_out(full_light, FULL_LIGHT_LOW);
	full_light_delay(10);
}

static void ws2812b_send_zero(full_light_t * const full_light)
{
	ws2812b_data_out(full_light, FULL_LIGHT_HIGH);
	full_light_delay(5);
	
	ws2812b_data_out(full_light, FULL_LIGHT_LOW);
	full_light_delay(10);
}

void full_light_ctl(full_light_t *const full_light, uint8_t green, uint8_t red, uint8_t blue)
{
	ws2812b_reset(full_light);
	
	uint8_t *tmp_data[] = {&green, &red, &blue};
	
	for(int i = 0; i < full_light->numbers; i += 1)
	{
		for(int j = 0; j < (sizeof(tmp_data) << 1); j += 1)
		{
			if(*tmp_data[j / 8] & (0x80 >> (j % 8)))
				ws2812b_send_one(full_light);
			else
				ws2812b_send_zero(full_light);
		}
	}
}

int full_light_init(full_light_t *const full_light)
{
	if(!full_light)
		return -1;
	
	rcu_periph_clock_enable(full_light->hw_info.gpio_rcu);
	
	gpio_bit_set(full_light->hw_info.gpio, full_light->hw_info.pin);
	gpio_init(full_light->hw_info.gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, full_light->hw_info.pin);
	
	return 0;
}
