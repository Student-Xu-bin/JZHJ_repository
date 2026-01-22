#include "drv_human.h"

int human_read(void)
{
	int temp = 0;
	
	if(HUMAN_1_DET() == RESET)
		temp |= 0x01;
	
	if(HUMAN_5_DET() == RESET)
		temp |= 0x02;
	
	return temp;
}

void human_init(void)
{
	dev_human_gpio_init();
}
