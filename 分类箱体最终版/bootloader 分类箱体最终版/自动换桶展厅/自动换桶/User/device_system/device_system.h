#ifndef __DEVICE_SYSTEM_H__
#define __DEVICE_SYSTEM_H__

#include "config.h"
#include "device.h"
#include "driver.h"

device_t *device_open(const char *name);
void devices_drivers_init(void);
device_t *device_get_device_hw_info(char *name);

#endif /* __DEVICE_SYSTEM_H__ */
