#include "gd32f30x.h"
#include "delay.h"
#include "garbage.h"

int main(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	__set_PRIMASK(1);
	
	delay_init();
	garbage_init();
	garbage_task();

	while(1)
	{
		
	}
}

