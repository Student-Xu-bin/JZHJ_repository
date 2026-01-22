#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "door.h"
#include "bags_broken.h"
#include "74hc595.h"
#include "cd4051.h"
#include "autochange.h"
#include "FreeRTOS.h"

#define 	_bit_left_move(n) 	( 1 << (n) )

#define 	DOOR_OPEN 				_bit_left_move(0)
#define 	DOOR_CLOSE 				_bit_left_move(1)
#define 	DOOR_STOP 				_bit_left_move(2)
#define 	STEPPER_ZERO 			_bit_left_move(3)
#define 	STEPPER_THROW_1			_bit_left_move(4)
#define 	STEPPER_THROW_2			_bit_left_move(5)
#define 	STEPPER_THROW_3			_bit_left_move(6)
#define 	STEPPER_THROW_AUTO 		_bit_left_move(7)
#define 	STEPPER_FULL_DETECT 	_bit_left_move(8)
#define 	GS_ENABLE 				_bit_left_move(9)
#define 	GS_DISABLE 				_bit_left_move(10)
#define 	WEIGHT_READ 			_bit_left_move(11)
#define 	THROW_GEBIN				_bit_left_move(12)
#define 	DOOR_KEY_OPEN 			_bit_left_move(13)
#define 	_BAGS_BROKEN_READY 		_bit_left_move(14)
#define 	_BAGS_BROKEN_RESET 		_bit_left_move(15)
#define 	_BAGS_BROKEN_WORK 		_bit_left_move(16)
#define 	_BAGS_OVER 				_bit_left_move(17)
#define 	STEPPER_THROW_4			_bit_left_move(18)

#define 	DOOR_ALL_EVENTS 		( 0x1ffff | (3 << 17) )



typedef struct channel_ {
	door_t 					door;			// 门板
	autochange_t   			autochange; 	// 自动换桶
	bagsbroken_t 			*bags_broken;	// 如果通道没有破袋设备，请指向NULL, 如果有破袋设备，在初始化之前赋值-1

	SemaphoreHandle_t 		door_mutex;
	SemaphoreHandle_t 		autochange_mutex;
	TaskHandle_t 			task_handle;
	TaskHandle_t 			detect_handle;
	
	enum channel_enum_t 	no;				// 通道编号
} channel_t;

extern channel_t channel[];

int channel_init(enum channel_enum_t no);
void task_channel(void *arg);
#endif
