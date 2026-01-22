#ifndef __BAGS_BROKEN_H__
#define __BAGS_BROKEN_H__

#include "solenoid_valve.h"
#include "74hc595.h"

#include "FreeRTOS.h"
#include "task.h"

extern void delay_nms(uint32_t);
#define bags_broken_delay(x) 	vTaskDelay(x)//vTaskDelay(x)

enum bags_broken_state_enum_t {
	BAGS_BROKEN_IDLE = 0, BAGS_BROKEN_BUSY, BAGS_BROKEN_READY
};

typedef struct bagsbroken_ {
	solenoid_valve_t 				solenoid_valve_horizontal_move;			// 四个电磁阀
	solenoid_valve_t 				solenoid_valve_vertical_move;
	solenoid_valve_t 				solenoid_valve_needling;
	solenoid_valve_t 				solenoid_valve_platform;
	
	uint8_t 						shake_times;							// 抖动次数
	uint8_t 						vertical_move_up_div;					// 垂直运动延时分频系数
	uint16_t 						shake_up_delay_time;					// 抖动上升延时
	uint16_t 						shake_down_delay_time;					// 抖动下降延时
	uint16_t 						needling_delay_time;					// 针刺延时
	uint32_t 						horizontal_move_delay_time;  			// 水平移动延时
	uint32_t 						vertical_move_delay_time;  				// 垂直移动延时
	
	volatile enum bags_broken_state_enum_t 	state;

} bagsbroken_t;

//int bags_broken_init(bagsbroken_t *bb);
void bags_broken_ready(bagsbroken_t *bb);
void bags_broken_work(bagsbroken_t *bb);
void bags_broken_reset(bagsbroken_t *bb);
uint8_t bags_broken_get_numbers(void);
int bags_broken_register(bagsbroken_t *bb, enum channel_enum_t no);
enum bags_broken_state_enum_t bags_broken_get_state(bagsbroken_t *bb);
void bags_broken_over(bagsbroken_t *bb);

#endif
