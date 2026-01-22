#ifndef __CREATE_TASK_H__
#define __CREATE_TASK_H__

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "74hc595.h"
#include "semphr.h"
#include "delay.h"
#include "stepper.h"
#include <string.h>
#include "cd4051.h"
#include "manled.h"
#include "mankey.h"
#include "timers.h"
#include "event_groups.h"
#include "modbus_app.h"

#define MAX_DOOR_NUM 	6

#define DEBUG

#define 	SCREEN_RS485  	(1 << 0)
#define 	WEIGHT_RS485  	(1 << 1)
#define 	WEIGHT_CH1_OK	(1 << 2)
#define 	WEIGHT_CH2_OK	(1 << 3)
#define 	WEIGHT_CH3_OK	(1 << 4)

#define STEPPER_FULL_STATE_DETECT_STEPS 	10000

typedef enum {
	CMD_IDLE, CMD_OPEN_DOOR, CMD_CLOSE_DOOR, CMD_READ_WEIGHT, CMD_THROW, CMD_BOOM_READY, CMD_BOOM
} door_command_enum_t;

//extern SemaphoreHandle_t 	cd4051SeBinarySemaphore;
//extern SemaphoreHandle_t 	_74hc595_mutex;

#endif
