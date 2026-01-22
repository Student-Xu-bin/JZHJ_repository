#include <string.h>
#include "device_system.h"

static void device_init_start(void)
{
    return;
}
INIT_EXPORT(device_init_start, "0")

static void device_init_end(void)
{
    return;
}
INIT_EXPORT(device_init_end, "0.end")

static void driver_init_start(void)
{
    return;
}
INIT_EXPORT(driver_init_start, "1")

static void driver_init_end(void)
{
    return;
}
INIT_EXPORT(driver_init_end, "1.end")

static device_t *device_driver_match(const char *name)
{
	device_t *device_ = device_get_list();
	
	if(!device_)
		return 0;
	
	while(device_) /* 找设备信息 */
	{
		if(strcmp(name, device_->name))
		{
			device_ = device_->next;
		}
		else
		{
			break;
		}
	}
	
	if(!device_)
		return 0;
	
	driver_t *driver_ = driver_get_list();
	
	while(driver_) /* 找配套驱动 */
	{
		if(strcmp(device_->driver_name, driver_->name))
		{
			driver_ = driver_->next;
		}
		else /* 找到后open */
		{
			if(driver_->open(device_) < 0)
				return 0;
			
			break;
		}
	}
	
	return device_;
}

device_t *device_open(const char *name)
{
	if((name == 0) || (name[0] == 0))
		return 0;

	return device_driver_match(name);
}

void devices_drivers_init(void)
{	
	volatile const init_fn_t *fn_ptr;

	/* device auto register */
    for (fn_ptr = &__init_device_init_start + 1; fn_ptr < &__init_device_init_end; fn_ptr++)
    {
        (*fn_ptr)();
    }
	
	/* driver auto register */
	for (fn_ptr = &__init_driver_init_start + 1; fn_ptr < &__init_driver_init_end; fn_ptr++)
    {
        (*fn_ptr)();
    }
}
