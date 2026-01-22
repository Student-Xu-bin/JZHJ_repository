#include "driver.h"
#include "device.h"

static driver_t *__drivers;

int driver_register(driver_t *drv)
{
	drv->next = __drivers;
	__drivers = drv;
	
	return 0;
}

int driver_init(char *name)
{
	if((name == 0) || (name[0] == 0))
		return 0;
	
	return 0;
}

driver_t *driver_get_list(void)
{
	return __drivers;
}
