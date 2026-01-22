#ifndef __INFRARED_H__
#define __INFRARED_H__

#include "gd32f30x.h"
#include "cd4051.h"

#define 	IR_ON 		SET
#define 	IR_OFF 		RESET

FlagStatus infrared_detect(uint8_t);

#endif
