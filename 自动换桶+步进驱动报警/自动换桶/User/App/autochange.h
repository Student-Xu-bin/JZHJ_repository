#ifndef __AUTOCHANGE_H__
#define __AUTOCHANGE_H__

#include "linear_actuator.h"
#include "stepper.h"
#include "jzhj_config.h"

#define AUTO_FIND 		-1.0f

#define auto_change_forward(x, y) 		gpio_bit_set(x, y)
#define auto_change_reverse(x, y) 		gpio_bit_reset(x, y)

#define FORWARD 		SET
#define FANXIANG 		RESET

#define WAIT_FOR_STEPPER(x) 		xSemaphoreTake((x)->autochange.stepper.idle_semaphore_handle, portMAX_DELAY);

enum {
	DUSTBIN_1 = 0,
	DUSTBIN_2,
	DUSTBIN_3,
	DUSTBIN_4,
};

typedef enum {
	zero_task = 1, no_zero_task = 0
}Zero_back_t;

typedef struct autochange_ {
	//infrared_t 					ir_distance; 						// 满桶测距
	linear_actuator_t 			linear_actuator;					// 推杆
	//proximity_switch_t			zero_ps;							// 零点接近开关
	//weight_t 					weight;								// 称重传感器
	stepper_t 					stepper; 							// 步进电机
	Zero_back_t					Zero_back;
	FlagStatus 					zero_dir_flag;
	float 						f_total_distance;					// 自动换桶总距离
	float 						f_dustbin_postion[DUSTBIN_NUMBERS];	// 每个垃圾桶的位置
	float 						f_full_detect_distance;
	uint8_t 					full_state; 						// 满桶状态  位从低到高分别对应桶位1-DUSTBIN_NUMBERS
	uint32_t 					weight;
} autochange_t;

void stepper_work(autochange_t *autochange, FlagStatus s, float disc);
int autochange_init(autochange_t *a, uint8_t no);
void stepper_set(stepper_t *stepper, FlagStatus s, uint32_t disc, uint8_t flag);

#endif
