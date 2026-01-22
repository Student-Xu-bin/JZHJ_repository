#include "gd32f30x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "systick.h"
#include <stdio.h>

/*
* 	1.回零
*   2.自动投放
*   3.按桶位置投放
*   4.全局检测满桶状态
*   5.破袋
*   6.打开投口
*   7.关闭投口
*   8.称重
*   9.
*/
#include "fulllight.h"
#include "delay.h"
#include "74hc595.h"
#include "linear_actuator.h"
#include "solenoid_valve.h"
#include "bags_broken.h"
#include "stepper.h"
#include "autochange.h"

#include "rs485.h"
#include "modbus_app.h"
#include "manled.h"
#include "mankey.h"

extern uint16_t g_acc_stepper_S[];

int main(void)
{
	_74hc595_init();
	stepper_calculate_S(g_acc_stepper_S, 1000, 110, STEPPER_ACC_MAX);
	delay_init();
	rs485_init();
	man_led_init();
	man_key_init();
	ModbusAppInit();
	
	extern void task_create(void *arg);
	if(xTaskCreate(task_create, "task_create", 512, NULL, 1, NULL) == pdPASS)
		vTaskStartScheduler();
	
	while(1)
	{
		printf("Task Create Falied\n");
	}
}
