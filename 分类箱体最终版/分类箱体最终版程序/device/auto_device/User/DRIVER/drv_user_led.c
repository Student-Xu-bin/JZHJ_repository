#include "drv_user_led.h"

void user_led_write(uint8_t state)
{
	switch(state)
	{
		case 0:
			USER_LED_OFF();
			break;
		case 1:
			USER_LED_ON();
			break;
		default:
			break;
	}
}

void user_led_init(void)
{
	user_led_gpio_init();
}
