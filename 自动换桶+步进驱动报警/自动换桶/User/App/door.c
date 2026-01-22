#include "door.h"

int door_init(door_t *door)
{
	if(!door)
		return -1;
	
	linear_actuator_init(&door->linear_actuator);
	full_light_init(&door->full_light);
	
	door->ir_grating_enable_flag = IR_DISABLE;
	door->ps_detect_flag 		 = PROXIMITY_SWITCH_OFF;
	
	return 0;
}
