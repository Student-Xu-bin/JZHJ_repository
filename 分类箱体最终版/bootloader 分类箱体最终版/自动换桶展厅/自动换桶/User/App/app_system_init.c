#include "device_system.h"
#include "app_system_init.h"
#include "delay.h"
#include "modbus_app.h"
#include "74hc595.h"
#include "app_adc.h"
#include "task.h"
#include "app_tasks.h"


#if 0
static device_t *uart_ttl;
static device_t *motor[CHANNEL_NUMBERS];
static device_t *full_light[CHANNEL_NUMBERS];
#endif

device_t *uart_ttl;
device_t *full_light[WORK_TASK_NUMBERS];
device_t *motor[WORK_TASK_NUMBERS];

TaskHandle_t task_detect_handle;
TaskHandle_t task_modbus_handle;
TaskHandle_t tasks_handle[WORK_TASK_NUMBERS];
TaskHandle_t tasks_motor_current[WORK_TASK_NUMBERS];
SemaphoreHandle_t uart_ttl_mutex;
QueueHandle_t works_queue;

uint16_t adc_zero[WORK_TASK_NUMBERS] = {0};

static void app_vcc_init(void)
{
	switch(CHANNEL_NUMBERS)
	{
	case 6 :
		_74hc595_ctl(CHANNEL_6, VCC_5V, ON, 0);
	case 5 :
		_74hc595_ctl(CHANNEL_5, VCC_5V, ON, 0);
	case 4 :
		_74hc595_ctl(CHANNEL_4, VCC_5V, ON, 0);
	case 3 :
		_74hc595_ctl(CHANNEL_3, VCC_5V, ON, 0);
	case 2 :
		_74hc595_ctl(CHANNEL_2, VCC_5V, ON, 0);
	case 1 :
		_74hc595_ctl(CHANNEL_1, VCC_5V, ON, 1);
		break;
	default :
		break;
	}
}

static void devices_open(void)
{
	uart_ttl = device_open("uart_ttl");
	
	switch(CHANNEL_NUMBERS)
	{
	case 6 :
		motor[5] = device_open("motor6");
	case 5 :
		motor[4] = device_open("motor5");
	case 4 :
		motor[3] = device_open("motor4");
	case 3 :
		motor[2] = device_open("motor3");
	case 2 :
		motor[1] = device_open("motor2");
	case 1 :
		motor[0] = device_open("motor1");
		break;
	default :
		break;
	}
	
	switch(CHANNEL_NUMBERS)
	{
	case 6 :
		full_light[5] = device_open("full_light6");
	case 5 :
		full_light[4] = device_open("full_light5");
	case 4 :
		full_light[3] = device_open("full_light4");
	case 3 :
		full_light[2] = device_open("full_light3");
	case 2 :
		full_light[1] = device_open("full_light2");
	case 1 :
		full_light[0] = device_open("full_light1");
		break;
	default :
		break;
	}
}

static void adc_get_zero(void)
{
	for(int j = 0; j < 500; j ++)
	{
		for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		{
			adc_zero[i] += adc_get_value(i);
		}
		delay_nms(10);
	}
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
	{
		adc_zero[i] /= 500;
	}
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
	{
		printf("ch%d = %d\n", i, adc_zero[i]);
		
	}
}

void app_system_init(void)
{
	devices_drivers_init();
	
	delay_init();
	_74hc595_init();
	app_vcc_init();
	
	devices_open();

	ModbusAppInit();
	adc_config();//adc_init();

	adc_get_zero();
	task_motor_current_init(0);
	uart_ttl_mutex = xSemaphoreCreateMutex();
	
	for(int i = 0; i < WORK_TASK_NUMBERS; i ++)
	{
		xTaskCreate(task_motor_current_detect, "abc", 128, (void *)motor[i], 11, &tasks_motor_current[i]);
	}
#if 0
	works_queue = xQueueCreate(10, sizeof(queue_package_t));
	if(!works_queue)
		printf("NULL!\n");
#endif
	xTaskCreate(task_modbus, "modbus", 128, NULL, 10, &task_modbus_handle);
	xTaskCreate(task_detect, "detect", 128, NULL, 10, &task_detect_handle);

#if 0
	for(int i = 0; i < WORK_TASK_NUMBERS; i ++)
	{
		xTaskCreate(task_work, "works", 512, NULL, 10, &tasks_handle[i]);
	}
#endif
	//xTaskCreate(task_motor_current_init, "k", 1024, NULL, 10, NULL);
	

	vTaskStartScheduler();
}
