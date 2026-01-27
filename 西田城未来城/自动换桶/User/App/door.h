#ifndef __DOOR_H__
#define __DOOR_H__

#include "linear_actuator.h"
#include "fulllight.h"
#include "74hc595.h"

enum ir_enable_enum_t {
	IR_ENABLE, IR_DISABLE
};

enum proximity_switch_detect_enum_t {
	PROXIMITY_SWITCH_ON, PROXIMITY_SWITCH_OFF
};

typedef struct door_ {
	enum ir_enable_enum_t 					ir_grating_enable_flag; // 光栅使能标志位
	enum proximity_switch_detect_enum_t 	ps_detect_flag;
	//infrared_t 				ir_grating; 		// 光栅
	linear_actuator_t 						linear_actuator;		// 推杆
	full_light_t 							full_light;				// 满桶灯
	//proximity_switch_t 		open_ps;			// 推杆开接近开关
	//proximity_switch_t		close_ps;			// 推杆关接近开关
	//key_t 					key;				// 按钮
} door_t;

int door_init(door_t *door);

#endif
