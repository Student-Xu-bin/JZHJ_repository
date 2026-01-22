#include <string.h>
#include "config.h"
#include "device.h"

static device_t *__devices;

int device_register(device_t *dev)
{
	dev->next = __devices;
	__devices = dev;
	
	return 0;
}

device_t *device_get_device_hw_info(char *name)
{
	if((name == 0) || (name[0] == 0))
		return 0;
	
	if(!__devices)
		return 0;
	
	device_t *ret_dev = __devices;
	
	while(ret_dev)
	{
		if(strcmp(name, ret_dev->name))
		{
			ret_dev = ret_dev->next;
		}
		else
		{
			break;
		}
	}
	
	return ret_dev;
}

device_t *device_get_list(void)
{
	return __devices;
}
