#include "gd32f30x.h"
#include "app_system_init.h"
#include "device_system.h"
#include "app_adc.h"

extern device_t *uart_ttl;
static uint8_t full_state[CHANNEL_NUMBERS] = {0};
static uint8_t open_state[CHANNEL_NUMBERS] = {0};
static uint8_t close_state[CHANNEL_NUMBERS] = {0};
#if 1 //mmm
static uint16_t adc_threshold_value[CHANNEL_NUMBERS] = {0};
#endif
void full_light_green(void *arg)
{
	device_t *pdev = (device_t *)arg;
	
	taskENTER_CRITICAL();
	pdev->ops->ioctl(pdev, CMD_FULL_LIGHT_GREEN, 0);
	taskEXIT_CRITICAL();
}

void full_light_red(void *arg)
{
	device_t *pdev = (device_t *)arg;

	taskENTER_CRITICAL();
	pdev->ops->ioctl(pdev, CMD_FULL_LIGHT_RED, 0);
	taskEXIT_CRITICAL();
}

void motor_open(void *arg)
{
	device_t *pdev = (device_t *)arg;
	
	pdev->ops->ioctl(pdev, CMD_MOTOR_ON, 0);
}

void motor_close(void *arg)
{
	device_t *pdev = (device_t *)arg;
	
	pdev->ops->ioctl(pdev, CMD_MOTOR_OFF, 0);
}

void motor_stop(void *arg)
{
	device_t *pdev = (device_t *)arg;
	
	pdev->ops->ioctl(pdev, CMD_MOTOR_STOP, 0);
}

uint8_t app_task_get_full_state(uint8_t no)
{
	return full_state[no];
}

uint8_t app_task_get_open_state(uint8_t no)
{
	return open_state[no];
}

uint8_t app_task_get_close_state(uint8_t no)
{
	return close_state[no];
}
#if 1 //mmm
void task_motor_current_init(void *arg)
{
	extern void delay_nms(uint32_t);
	uint32_t adc_value[CHANNEL_NUMBERS] = {0};
	uint16_t tmp = 0;
	uint32_t count__[CHANNEL_NUMBERS] = {0};
	uint8_t tmp_ = 0;
	
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
	{
		motor[i]->ops->ioctl(motor[i], CMD_MOTOR_ON, 0);
	}
	motor[1]->ops->ioctl(motor[1], CMD_MOTOR_ON, 0);
	//pdev->ops->ioctl(pdev, CMD_MOTOR_ON, 0); /* 先打开投口 */
	delay_nms(500);
	uint8_t tmp_count[CHANNEL_NUMBERS] = {0};
	uint32_t count = 0;
	while(1)
	{
		for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		{
			if(tmp_count[i] >= 200)
			{
				tmp_count[i] = 0xff;
				continue;
			}
			tmp = adc_get_value(i);
			if(tmp < (adc_zero[i] + 10))
			{
				tmp_count[i] ++;
			}
			else
			{
				if(tmp_count[i])
					tmp_count[i] --;
			}
		}
		for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		{
			if(tmp_count[i] == 0xff)
				count ++;
		}
		if(count >= CHANNEL_NUMBERS)
			break;
		
		delay_nms(10);
				
	}	/* 等投口到位 */
	delay_nms(500);

	for(int j = 0; j < CHANNEL_NUMBERS; j ++)
	{
		motor[j]->ops->ioctl(motor[j], CMD_MOTOR_OFF, 0);
		tmp_count[j] = 0;
	}
	motor[1]->ops->ioctl(motor[1], CMD_MOTOR_OFF, 0);

	delay_nms(500);
	
	while(1)
	{
		for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		{
			if(tmp_count[i] >= 200)
			{
				tmp_count[i] = 0xff;
				continue;
			}
			tmp = adc_get_value(i);
			if(tmp >= 100)
			{
				adc_value[i] += tmp;
				count__[i] += 1;
				
				if(tmp_count[i])
					tmp_count[i] --;
			}
			else if(tmp < (adc_zero[i] + 10))
			{
				tmp_count[i] ++;
			}
		}
		for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		{
			if(tmp_count[i] == 0xff)
				tmp_ ++;
		}
		if(tmp_ >= CHANNEL_NUMBERS)
			break;
		
		delay_nms(10);
	}
	
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
	{
		if(count__[i])
			adc_threshold_value[i] = adc_value[i] / count__[i];
		printf("adc_threshold_value = %d\n", adc_threshold_value[i]);
	}
}
#endif
static void task_detect_open_devices(device_t *key[], device_t *ir[])
{
	switch(CHANNEL_NUMBERS)
	{
	case 6 :
		key[5] = device_open("key6");
		ir[5]  = device_open("ir6");
	case 5:
		key[4] = device_open("key5");
		ir[4]  = device_open("ir5");
	case 4 :
		key[3] = device_open("key4");
		ir[3]  = device_open("ir4");
	case 3:
		key[2] = device_open("key3");
		ir[2]  = device_open("ir3");
	case 2 :
		key[1] = device_open("key2");
		ir[1]  = device_open("ir2");
	case 1:
		key[0] = device_open("key1");
		ir[0]  = device_open("ir1");
		break;
	default :
		break;
	}
}

void task_detect(void *arg)
{
	device_t *key[CHANNEL_NUMBERS];
	device_t *ir[CHANNEL_NUMBERS];
	
	task_detect_open_devices(key, ir);
	
	TickType_t xLastWakeTime;
    const TickType_t delay_50ms = pdMS_TO_TICKS(50);
	xLastWakeTime = xTaskGetTickCount();
	
	state_enum_t key_cur_state[CHANNEL_NUMBERS] = {KEY_LOOSE, KEY_LOOSE, KEY_LOOSE, KEY_LOOSE, KEY_LOOSE, KEY_LOOSE};
	state_enum_t key_old_state[CHANNEL_NUMBERS] = {KEY_LOOSE, KEY_LOOSE, KEY_LOOSE, KEY_LOOSE, KEY_LOOSE, KEY_LOOSE};
	state_enum_t ir_state  = IR_EMPTY;
#if 0 //mmm	
	queue_package_t tmp = {0};

	for(int i = 0; i < 1; i ++)
	{
		tmp.data = (void *)motor[i];
		tmp.task_cb = task_motor_current_init;
		//xQueueSend(works_queue, &tmp, 500);
		//tmp.data = (void *)motor[i];
		//tmp.task_cb = motor_open;
		xQueueSend(works_queue, &tmp, 500);
	}
#endif	
	while(1)
	{
		for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		{
			key[i]->ops->read(key[i], &key_cur_state[i], 1);
			ir[i]->ops->read(ir[i], &ir_state, 1);
			
			if((key_cur_state[i] == KEY_PRESSED) && (key_old_state[i] == KEY_LOOSE))
			{
				xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
				uart_ttl->ops->write(uart_ttl, (uint8_t []){0xee, i + 1}, 2);
				xSemaphoreGive(uart_ttl_mutex);
				/* mmmm */
				
				key_old_state[i] = key_cur_state[i];
			}
			else if((key_cur_state[i] == KEY_LOOSE) && (key_old_state[i] == KEY_PRESSED))
			{
				key_old_state[i] = KEY_LOOSE;
			}
			
			if(ir_state == IR_FULL)
			{
				full_state[i] = 1;
			}
			else
			{
				full_state[i] = 0;
			}
			/*****/
			switch(full_light[i]->state){
#if (JZHJ_FULL_LIGHT == WS2812B)
			case LIGHT_GREEN:
				full_light[i]->ops->ioctl(full_light[i], CMD_FULL_LIGHT_GREEN, NULL);
				break;
			case LIGHT_RED:
				full_light[i]->ops->ioctl(full_light[i], CMD_FULL_LIGHT_RED, NULL);
				break;
			case LIGHT_BLUE:
				full_light[i]->ops->ioctl(full_light[i], CMD_FULL_LIGHT_BLUE, NULL);
				break;
			case LIGHT_YELLOW:
				full_light[i]->ops->ioctl(full_light[i], CMD_FULL_LIGHT_YELLOW, NULL);
				break;
#elif (JZHJ_FULL_LIGHT == RED_LIGHT)
			case LIGHT_OFF:
				full_light[i]->ops->ioctl(full_light[i], CMD_FULL_LIGHT_OFF, NULL);
				break;
			case LIGHT_ON:
				full_light[i]->ops->ioctl(full_light[i], CMD_FULL_LIGHT_ON, NULL);
				break;
#endif
			default:
				break;
			}
		}
		
		vTaskDelayUntil(&xLastWakeTime, delay_50ms);
	}
}

void task_modbus(void *arg)
{
	extern void ModbusTask(void);
	
	while(1)
	{
		ModbusTask();
		vTaskDelay(1);
	}
}

static int wait_door_close(device_t *pdev)
{
	uint16_t count = 0;
	int motor_no   = pdev->name[5] - '0' - 1;
	int32_t tmp_ = 0;
	int32_t tmp_old_ = 0;
	uint16_t count_stop = 0;
	while(1)
	{
		tmp_ = (int32_t)adc_get_value(motor_no) - (int32_t)adc_threshold_value[motor_no];
		if((tmp_ > (adc_threshold_value[motor_no] * MOTOR_OVERLOAD_COEFFICIENT)) && (tmp_old_ > (adc_threshold_value[motor_no] * MOTOR_OVERLOAD_COEFFICIENT)))//mmmm150)
		{
			count ++;
			if(count_stop)
				count_stop --;
		}
		else if((tmp_ < (adc_zero[motor_no] + MOTOR_CLSOE_ZERO_DRIFT)) && (tmp_old_ < (adc_zero[motor_no] + MOTOR_CLSOE_ZERO_DRIFT)))
		{
			count_stop ++;
			if(count)
				count --;
		}
		else
		{
			if(count_stop)
				count_stop --;
			if(count)
				count --;
		}
		if(count >= MOTOR_ERROR_STOP_TIMES)
		{
			pdev->ops->ioctl(pdev, CMD_MOTOR_STOP, 0);
			return -1;
		}
		if(count_stop >= MOTOR_NO_ERROR_BREAK_TIMES)
			break;
		tmp_old_ = tmp_;
		vTaskDelay(10);
	}
	
	return 0;
}
#if 1
static int wait_door_open(device_t *pdev)
{
	int32_t tmp_old_ = 0;
	int motor_no   = pdev->name[5] - '0' - 1;
	int32_t tmp_ = 0;
	uint16_t count_stop = 0;
	while(1)
	{
		tmp_ = (int32_t)adc_get_value(motor_no) - (int32_t)adc_threshold_value[motor_no];
		if((tmp_ < (adc_zero[motor_no] + MOTOR_OPEN_ZERO_DRIFT)) && (tmp_old_ < (adc_zero[motor_no] + MOTOR_OPEN_ZERO_DRIFT)))
		{
			count_stop ++;
		}
		else
		{
			if(count_stop)
				count_stop --;
		}
		if(count_stop >= MOTOR_NO_ERROR_BREAK_TIMES)
			break;
		
		tmp_old_ = tmp_;
		vTaskDelay(10);
	}
	
	return 0;
}
#endif
void task_motor_current_detect(void *arg)
{
	device_t *pdev = (device_t *)arg;
	int motor_no   = pdev->name[5] - '0' - 1;
	
	BaseType_t 	ret = pdTRUE;
	uint32_t 	read_event = 0;
	int ret_tmp = 0;
	while(1)
	{
		ret = xTaskNotifyWait(~DOOR_ALL_EVENTS, 0xffffffff, &read_event, portMAX_DELAY);
		if(ret == pdFALSE)
			continue;
		
		switch(read_event & DOOR_ALL_EVENTS)
		{
		case DOOR_CLOSE_START:
			gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
			gpio_bit_reset(pdev->hw_info[1].port, pdev->hw_info[1].pin);
			gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
			gpio_bit_set(pdev->hw_info[1].port, pdev->hw_info[1].pin);
			vTaskDelay(500);
		
			ret_tmp = wait_door_close(pdev);
			gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
			gpio_bit_reset(pdev->hw_info[1].port, pdev->hw_info[1].pin);
			if(-1 == ret_tmp) // 过流
			{
				xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
				uart_ttl->ops->write(uart_ttl, (uint8_t []){0x0e, motor_no + 1}, 2);
				xSemaphoreGive(uart_ttl_mutex);
			}
			else
			{
				//open_state[motor_no] = 0;
				close_state[motor_no] = 1;
			}
			open_state[motor_no] = 0;
			//pdev->ops->ioctl(pdev, CMD_MOTOR_STOP, 0);
			break;
		case DOOR_OPEN_START:
			gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
			gpio_bit_reset(pdev->hw_info[1].port, pdev->hw_info[1].pin);
			gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);
			gpio_bit_reset(pdev->hw_info[1].port, pdev->hw_info[1].pin);
		
			vTaskDelay(500);
			wait_door_open(pdev);
			open_state[motor_no] = 1;
			close_state[motor_no] = 0;
			pdev->ops->ioctl(pdev, CMD_MOTOR_STOP, 0);
			break;
		default :
			break;
		}
	}
}

#if 0
void task_work(void *arg)
{
	BaseType_t status;
	
	queue_package_t package = {0};
	
	while(1)
	{
		status = xQueueReceive(works_queue, &package, portMAX_DELAY);
		
		if(status == pdPASS)
		{
			if(package.task_cb)
				package.task_cb(package.data);
		}
	}
}
#endif
