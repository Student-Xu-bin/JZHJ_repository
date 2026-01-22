#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "config.h"

typedef struct device_t device_t;

typedef struct operations_t {
	int (*read)(device_t *pdev, void *buf, size_t count);
	int (*write)(device_t *pdev, const void *buf, size_t count);
	int (*ioctl)(device_t *pdev, uint32_t cmd, void *arg);
	int (*close)(device_t *pdev);
} operations_t;

#if (JZHJ_MCU == GD32F303)
#include "gd32f30x.h"

struct device_hw_t {
	char *name;
	uint32_t 			port;
	uint32_t 			pin;
	rcu_periph_enum     port_rcu;
	uint32_t 			private_data;
};

struct device_t {
	char *name;
	char *driver_name;
	struct device_hw_t *hw_info;
	operations_t *ops;
	state_enum_t 	state;
	struct device_t *next;
};

#endif /* JZHJ_MCU == GD32F303 */

int device_register(device_t *dev);
//device_t *device_open(const char *name);

device_t *device_get_list(void);

#endif /* __DEVICE_H__ */
