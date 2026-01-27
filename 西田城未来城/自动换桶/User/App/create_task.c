#include "create_task.h"
#include "channel.h"
#include "CD4051.h"
#include "jzhj_config.h"
#include "app.h"

TaskHandle_t modbus_taskhandle;
//TaskHandle_t shouzhen_task;
//SemaphoreHandle_t 	_74hc595_mutex;

static void task_modbus(void *arg)
{
	while(1)
	{
		ModbusTask();
		vTaskDelay(1);
	}
}

//void task_alm(void *arg)
//{
//	while(1){
//		stepper_alm_detect();
//		vTaskDelay(6000);
//	}
//}

extern void shouzhen_task_(void *arg);

void task_create(void *arg)
{
	taskENTER_CRITICAL();
	
	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		channel_init((enum channel_enum_t)i);
		xTaskNotify(channel[i].task_handle, DOOR_CLOSE, eSetBits);
	}
	
	xTaskCreate(task_modbus, "task_modbus", 128, NULL, 10, &modbus_taskhandle);
	//xTaskCreate(task_alm, "task_alm", 128, NULL, 10, NULL);
	//xTaskCreate(shouzhen_task_, "shouzhen", 128, NULL, 10, &shouzhen_task);
	//_74hc595_mutex = xSemaphoreCreateMutex();
	
	delay_nms(2000);
	
	taskEXIT_CRITICAL();
	
	//vTaskDelete(NULL);
	while(1)
	{
		vTaskDelay(1000);
	}
}
