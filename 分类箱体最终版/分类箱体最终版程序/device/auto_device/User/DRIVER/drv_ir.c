#include "drv_ir.h"

int ir_read(void)
{
	int temp = 0;
	
	if(IR_1_DET() == RESET)
		temp |= 0x01;
	
	if(IR_4_DET() == RESET)
		temp |= 0x02;
	
	if(IR_5_DET() == RESET)
		temp |= 0x04;
	
	return temp;
}

void ir_init(void)
{
	dev_ir_gpio_init();
}

