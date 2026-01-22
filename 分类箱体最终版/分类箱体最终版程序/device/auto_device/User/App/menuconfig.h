#ifndef __MENUCONFIG_H
#define __MENUCONFIG_H

#define SYS_NUM 6

#include "gd32f30x.h"

typedef struct{
	void (*init)(void);
	void (*write)(uint8_t state);
	int (*read)(void);
}interface;




#endif
