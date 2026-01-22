#include "drv_light.h"

void light_write(uint8_t channel, uint8_t state)
{
	switch(channel)
	{
		case 0:
			_74hc595_ctl(CHANNEL_1, RED_LED, state, 1);
		
	}
	
}

void light_init(void)
{
	for(int i = 0; i < 6; i++)
	{
		dev_ws2812b_gpio_init(&ws2812b[i]);
	}
}
