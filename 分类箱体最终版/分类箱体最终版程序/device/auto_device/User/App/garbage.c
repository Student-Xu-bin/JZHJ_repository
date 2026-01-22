#include "garbage.h"

interface user_led;
interface sw;
volatile uint8_t color[6][3] = {
	{0, 255, 0},
	{0, 255, 0},
	{0, 255, 0},
	{0, 255, 0},
	{0, 255, 0},
	{0, 255, 0}
};
SemaphoreHandle_t uart_ttl_mutex;
TaskHandle_t tasks_motor_current[6];
uint8_t ir_buf[6] = {0};

uint8_t get_ir_state(uint8_t channel)
{
	return ir_buf[channel];
}

void current_zero_init(void)
{
	for(int i = 0; i < 500; i++)
	{
		for(int i = 0; i < 6; i ++)
			motor[i].zero_current += adc_get_value(i);
		delay_nms(10);
	}
	for(int i = 0; i < 6; i ++)
		motor[i].zero_current /= 500;
}

static int wait_door_close(motor_t *pdev)
{
	uint16_t count = 0;
	int motor_no   = pdev->no;
	int32_t tmp_ = 0;
	int32_t tmp_old_ = 0;
	uint16_t count_stop = 0;
	while(1)
	{
		tmp_ = (int32_t)adc_get_value(motor_no) - (int32_t)motor[motor_no].limit_currnet;
		if((tmp_ > (motor[motor_no].limit_currnet * MOTOR_OVERLOAD_COEFFICIENT)) && (tmp_old_ > (motor[motor_no].limit_currnet * MOTOR_OVERLOAD_COEFFICIENT)))
		{
			count ++;
			if(count_stop)
				count_stop --;
		}
		else if((tmp_ < (motor[motor_no].zero_current + MOTOR_CLSOE_ZERO_DRIFT)) && (tmp_old_ < (motor[motor_no].zero_current + MOTOR_CLSOE_ZERO_DRIFT)))
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
			motor_stop(pdev);
			return -1;
		}
		if(count_stop >= MOTOR_NO_ERROR_BREAK_TIMES)
			break;
		tmp_old_ = tmp_;
		vTaskDelay(10);
	}
	
	return 0;
}

static int wait_door_open(motor_t *pdev)
{
	int32_t tmp_old_ = 0;
	int motor_no   = pdev->no;
	int32_t tmp_ = 0;
	uint16_t count_stop = 0;
	while(1)
	{
		tmp_ = (int32_t)adc_get_value(motor_no) - (int32_t)motor[motor_no].limit_currnet;
		if((tmp_ < (motor[motor_no].zero_current + MOTOR_OPEN_ZERO_DRIFT)) && (tmp_old_ < (motor[motor_no].zero_current + MOTOR_OPEN_ZERO_DRIFT)))
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

static int motor_current_init(int motor_no)
{
	uint8_t tmp_count = 0;
	uint16_t tmp = 0;
	uint32_t adc_value = 0;
	uint32_t count__ = 0;

	motor_stop(&motor[motor_no]);
	vTaskDelay(10);
	motor_open(&motor[motor_no]);
	vTaskDelay(500);
	
	while(1){
		if(tmp_count >= 200)
			break;
		
		tmp = adc_get_value(motor_no);

		if(tmp < (motor[motor_no].zero_current + 10)){
			tmp_count++;
		}
		else{
			if(tmp_count)
				tmp_count--;
		}
		vTaskDelay(10);
	}
	
	vTaskDelay(500);
	
	motor_stop(&motor[motor_no]);
	vTaskDelay(10);
	motor_close(&motor[motor_no]);

	tmp_count = 0;
	
	vTaskDelay(500);
	
	while(1){
		if(tmp_count >= 200)
			break;

		tmp = adc_get_value(motor_no);

		if(tmp >= 100){
			adc_value += tmp;
			count__ += 1;
				
			if(tmp_count)
				tmp_count--;
		}
		else if(tmp < (motor[motor_no].zero_current + 10))
			tmp_count++;
		vTaskDelay(10);
	}
	if(count__)
		motor[motor_no].limit_currnet = adc_value / count__;

	motor_stop(&motor[motor_no]);
	
	return 1;
}

void task_motor_current_detect(void *arg)
{
	motor_t *pdev = (motor_t *)arg;
	int motor_no   = pdev->no;
	
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
			motor_stop(pdev);
			motor_close(pdev);
			vTaskDelay(500);
		
			ret_tmp = wait_door_close(pdev);
			motor_stop(pdev);
			if(-1 == ret_tmp) 
			{
				xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
				ttl_send_data((uint8_t []){0x0e, motor_no + 1}, 2);
				xSemaphoreGive(uart_ttl_mutex);
				
				motor[motor_no].error_flag = 1;
			}
			else
			{
				motor[motor_no].close_state = 1;
			}
			motor[motor_no].open_state = 0;
			break;
		case DOOR_OPEN_START:
			//if(pdev->no == 3) {
			//	ttl_send_data((uint8_t []){0xee, 06}, 2);
			//}
			motor_stop(pdev);
			motor_open(pdev);
		
			vTaskDelay(500);
			wait_door_open(pdev);
			motor[motor_no].open_state = 1;
			motor[motor_no].close_state = 0;
			motor_stop(pdev);
			break;
		case DOOR_GET_CURRENT:
			if(motor[motor_no].channel_init_flag == 0){
				if(motor_current_init(motor_no))
				{
					motor[motor_no].close_state = 1;
					motor[motor_no].open_state = 0;
					motor[motor_no].channel_init_flag = 1;
					motor[motor_no].error_flag = 0;
				}
			}
			break;
		default :
			break;
		}
	}
}

void garbage_init(void)
{
	ttl_init();
	_74hc165_gpio_init();
	_74hc595_init();
	motor_init();
	adc_init_config();
	current_zero_init();
	
	
	sw.init = sw_init;
	sw.read = sw_read;
	sw.init();
	
	uart_ttl_mutex = xSemaphoreCreateMutex();
	ModbusAppInit();
}

void user_led_task(void *arg)
{
	user_led.init = user_led_init;
	user_led.write = user_led_write;
	user_led.init();
	
	while(1)
	{
		user_led.write(0);
		vTaskDelay(200);
		user_led.write(1);
		vTaskDelay(500);
		
	}
}

void garbage_det(void *arg)
{
	int temp_key, temp_human, temp_ir;
	uint16_t temp_165;
	uint8_t light;
	TickType_t xLastWakeTime;
	const TickType_t delay_50ms = pdMS_TO_TICKS(50);
	uint8_t key_current_state[6] = {0};
	uint8_t key_last_state[6] = {0};
	uint8_t human_current_state[6] = {0};
	uint8_t human_last_state[6] = {0};
	
	key_init();
	human_init();
	ir_init();
	
	if((sw.read() & (1 << 1)) == 0)
	{
		light = 1;
		for(int i = 0; i < 6; i++)
			dev_ws2812b_gpio_init(&ws2812b[i]);
	}
	
	while(1)
	{
		temp_ir = ir_read();
		temp_human = human_read();
		temp_key = key_read();
		temp_165 = _74hc165_read_data();
		
		if(temp_ir & 0x01)
			ir_buf[0] = 1;
		else
			ir_buf[0] = 0;
		
		if(temp_ir & 0x02)
			ir_buf[3] = 1;
		else
			ir_buf[3] = 0;
		
		if(temp_ir & 0x04)
			ir_buf[4] = 1;
		else
			ir_buf[4] = 0;
		
		if(temp_165 & (1 << 2))
			ir_buf[2] = 1;
		else
			ir_buf[2] = 0;
		
		if(temp_165 & (1 << 5))
			ir_buf[1] = 1;
		else
			ir_buf[1] = 0;
		
		if(temp_165 & (1 << 15))
			ir_buf[5] = 1;
		else
			ir_buf[5] = 0;
		
		if(temp_human & 0x01)
			human_current_state[0] = 1;
		else
			human_current_state[0] = 0;
		
		if(temp_human & 0x02)
			human_current_state[4] = 1;
		else
			human_current_state[4] = 0;
		
		if(temp_165 & (1 << 1))
			human_current_state[2] = 1;
		else
			human_current_state[2] = 0;
		
		if(temp_165 & (1 << 4))
			human_current_state[3] = 1;
		else
			human_current_state[3] = 0;
		
		if(temp_165 & (1 << 6))
			human_current_state[1] = 1;
		else
			human_current_state[1] = 0;
		
		if(temp_165 & (1 << 14))
			human_current_state[5] = 1;
		else
			human_current_state[5] = 0;
		
		
		if(temp_key & 0x01)
			key_current_state[0] = 1;
		else
			key_current_state[0] = 0;
		
		if((key_current_state[0] & !key_last_state[0]) || (human_current_state[0] & !human_last_state[0]))
		{
			xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
			ttl_send_data((uint8_t []){0xee, 0x01}, 2);
			xSemaphoreGive(uart_ttl_mutex);
		}
		
		if(temp_key & 0x02)
			key_current_state[4] = 1;
		else
			key_current_state[4] = 0;
		
		if((key_current_state[4] & !key_last_state[4]) || (human_current_state[4] & !human_last_state[4]))
		{
			xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
			ttl_send_data((uint8_t []){0xee, 0x05}, 2);
			xSemaphoreGive(uart_ttl_mutex);
		}
		
		if(temp_165 & (1 << 13))
			key_current_state[5] = 1;
		else
			key_current_state[5] = 0;
		
		if(key_current_state[5] & !key_last_state[5])
		{
			xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
			ttl_send_data((uint8_t []){0xee, 0x06}, 2);
			xSemaphoreGive(uart_ttl_mutex);
		}
		
		if(temp_165 & (1 << 7))
			key_current_state[1] = 1;
		else
			key_current_state[1] = 0;
		
		if((key_current_state[1] & !key_last_state[1]) || (human_current_state[1] & !human_last_state[1]))
		{
			xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
			ttl_send_data((uint8_t []){0xee, 0x02}, 2);
			xSemaphoreGive(uart_ttl_mutex);
		}
		
		if(temp_165 & (1 << 3))
			key_current_state[3] = 1;
		else
			key_current_state[3] = 0;
		
		if((key_current_state[3] & !key_last_state[3]) || (human_current_state[3] & !human_last_state[3]))
		{
			xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
			ttl_send_data((uint8_t []){0xee, 0x04}, 2);
			xSemaphoreGive(uart_ttl_mutex);
		}
		
		if(temp_165 & 0x01)
			key_current_state[2] = 1;
		else
			key_current_state[2] = 0;
		
		if((key_current_state[2] & !key_last_state[2]) || (human_current_state[2] & !human_last_state[2]))
		{
			xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
			ttl_send_data((uint8_t []){0xee, 0x03}, 2);
			xSemaphoreGive(uart_ttl_mutex);
		}
		
		for(int i = 0; i < 6; i++)
		{
			if(light)
			{
				taskENTER_CRITICAL();
				ws2812b_ctl(&ws2812b[i], color[i][1], color[i][0], color[i][2]);
				taskEXIT_CRITICAL();
			}
			key_last_state[i] = key_current_state[i];
			human_last_state[i] = human_current_state[i];
		}
		
		vTaskDelayUntil(&xLastWakeTime, delay_50ms);
	}	
}

void modbus_task(void *arg)
{
	while(1)
	{
		ModbusTask();
		vTaskDelay(1);
	}
}

void wwdgt_init(void)
{
	rcu_osci_on(RCU_IRC40K);
	while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K)){
	}
	
	fwdgt_write_enable();
	fwdgt_config(2500, FWDGT_PSC_DIV16);
	fwdgt_write_disable();
	fwdgt_enable();
}
void task_dog_feed(void *arg)
{
	TickType_t xLastWakeTime;
	wwdgt_init();
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t xDelay10ms = pdMS_TO_TICKS(10);
	
	while(1)
	{
		fwdgt_write_enable();
		fwdgt_counter_reload();
		fwdgt_write_disable();
		
		vTaskDelayUntil(&xLastWakeTime, xDelay10ms);
	}
}

void garbage_task(void)
{
	for(int i = 0; i < 6; i ++)
	{
		xTaskCreate(task_motor_current_detect, "abc", 1024, (void *)&motor[i], 10, &tasks_motor_current[i]);
	}
	
	xTaskCreate(task_dog_feed, "dog_feed", 128, NULL, 10, NULL);
	xTaskCreate(garbage_det, "task_garbage_det", 128, NULL, 10, NULL);
	xTaskCreate(user_led_task, "task_user_led", 128, NULL, 10, NULL);
	xTaskCreate(modbus_task, "task_modbus", 128, NULL, 10, NULL);
	vTaskStartScheduler(); 
}
