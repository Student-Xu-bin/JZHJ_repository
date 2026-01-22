#include "drv_key.h"

int key_read(void)
{
	int temp = 0;
	
	if(KEY_1_DET() == RESET)
		temp |= 0x01;
	
	if(KEY_5_DET() == RESET)
		temp |= 0x02;
	
	return temp;
}

void key_init(void)
{
	dev_key_gpio_init();
}

