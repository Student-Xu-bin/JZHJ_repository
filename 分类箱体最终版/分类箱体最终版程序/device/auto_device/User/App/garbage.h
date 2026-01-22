#ifndef __GARBAGE_H
#define __GARBAGE_H

#include "menuconfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drv_user_led.h"
#include "delay.h"
#include "systick.h"
#include "drv_sw.h"
#include "dev_ws2812b.h"
#include "drv_ws2812b.h"
#include "74hc165.h"
#include "drv_key.h"
#include "semphr.h"
#include "usart.h"
#include "drv_human.h"
#include "drv_ir.h"
#include "modbus_app.h"
#include "74hc595.h"
#include "drv_motor.h"
#include "drv_adc.h"

#define 	MOTOR_OVERLOAD_COEFFICIENT 	0.15
#define 	MOTOR_CLSOE_ZERO_DRIFT		20
#define 	MOTOR_OPEN_ZERO_DRIFT		10
#define 	MOTOR_ERROR_STOP_TIMES		20
#define 	MOTOR_NO_ERROR_BREAK_TIMES	200
#define  	MOTOR_ERROR_VALUE			4000

#define 	DOOR_OPEN_START 				(1 << 0)
#define 	DOOR_CLOSE_START 				(1 << 1)
#define     DOOR_GET_CURRENT				(1 << 2)
#define 	DOOR_ALL_EVENTS					(0x07)

void garbage_init(void);
void garbage_task(void);

#endif
