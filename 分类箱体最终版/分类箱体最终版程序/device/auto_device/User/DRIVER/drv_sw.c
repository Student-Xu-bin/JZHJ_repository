#include "drv_sw.h"

void sw_init(void)
{
	sw_gpio_init();
}

int sw_read(void)
{
	int temp = 0;
	
	if(SW_READ_HUMAN() == RESET)
		temp |= 1;
	
	if(SW_READ_LIGHT() == RESET)
		temp |= (1<<1);
	
	return temp;
}
