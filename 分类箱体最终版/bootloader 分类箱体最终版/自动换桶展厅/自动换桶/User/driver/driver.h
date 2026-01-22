#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "config.h"
#include "device.h"

typedef struct driver_t driver_t;

struct driver_t {
	const char  	    *name;
	int (*open)(device_t *pdev);
	struct driver_t 	*next;
	void                *private_data;
};

int driver_register(driver_t *drv);
int driver_init(char *name);
driver_t *driver_get_list(void);

#endif /* __DRIVER_H__ */
