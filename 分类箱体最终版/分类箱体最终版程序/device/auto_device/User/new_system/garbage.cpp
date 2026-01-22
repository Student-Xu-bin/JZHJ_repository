#include "garbage_config.h"

garbage *garbage_system = nullptr;

static TaskHandle_t motor_task_handle[6] = {NULL};
SemaphoreHandle_t uart_ttl_mutex;
volatile uint16_t adc_values[6];

garbage *garbage::single_garbage = nullptr;

garbage *garbage::creat_garbage()
{
	if(!single_garbage)
	{
		single_garbage = new garbage();
	}
	return single_garbage;
}

void garbage_detect_task(void *arg)
{
	TickType_t xLastWakeTime;
	const TickType_t delay_50ms = pdMS_TO_TICKS(50);
	xLastWakeTime = xTaskGetTickCount();
	uint8_t current_error_flag[6] = {0};
	uint8_t old_error_flag[6] = {0};
	while(1)
	{
		for(int i = 0; i < 6; i++)
		{
			if(garbage_system->pitches[i]->key_read_status())
			{
				xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
				garbage_system->usart_send_data(UART3, (uint8_t []){0xee, static_cast<uint8_t>(i + 1)}, 2);
				xSemaphoreGive(uart_ttl_mutex);
			}
			
			taskENTER_CRITICAL();
			garbage_system->pitches[i]->full_light_ctl(garbage_system->pitches[i]->full_light_type, 
													   garbage_system->pitches[i]->full_light.r, 
													   garbage_system->pitches[i]->full_light.g, 
													   garbage_system->pitches[i]->full_light.b, 
													   garbage_system->pitches[i]->full_light_status);
			taskEXIT_CRITICAL();
			
			if((current_error_flag[i] == 1) && (old_error_flag[i] == 1))
			{
				xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
				garbage_system->usart_send_data(UART3, (uint8_t []){0x0e, static_cast<uint8_t>(i + 1)}, 2);
				xSemaphoreGive(uart_ttl_mutex);
			}
			if(garbage_system->pitches[i]->pitch_error_flag)
				current_error_flag[i] = 1;
			else
				current_error_flag[i] = 0;
			old_error_flag[i] = current_error_flag[i];
		}
		vTaskDelayUntil(&xLastWakeTime, delay_50ms);
	}
}

void motor_task(void *arg)
{
	int no = (int)arg;
	// std::unique_ptr<garbage_pitch> * door = (std::unique_ptr<garbage_pitch> *)arg;
	BaseType_t 	ret = pdTRUE;
	uint32_t 	read_event = 0;
	// (*door)->wait_pitch_open();
	while(1)
	{
		ret = xTaskNotifyWait(~DOOR_ALL_EVENTS, 0xffffffff, &read_event, portMAX_DELAY);
		if(ret == pdFALSE)
			continue;
		
		switch(read_event & DOOR_ALL_EVENTS)
		{
			case DOOR_OPEN_START:
				//if(garbage_system->pitches[no]->pitch_error_flag != 1)
					garbage_system->pitches[no]->wait_pitch_open();
				break;
			case DOOR_CLOSE_START:
				//if(garbage_system->pitches[no]->pitch_error_flag != 1)
					garbage_system->pitches[no]->wait_pitch_close();
				break;
			case DOOR_GET_CURRENT:
				//if(garbage_system->pitches[no]->current_init_flag == 0)
					garbage_system->pitches[no]->adc_get_benchmark_value();
				break;
			default:
				break;
		}
		read_event = 0;
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

void date_current_detect(void *arg)
{
	TickType_t xLastWakeTime;
	const TickType_t delay_50ms = pdMS_TO_TICKS(50);
	xLastWakeTime = xTaskGetTickCount();
	uint16_t temp = 0;
	uint32_t count = 0;
	
	while(1)
	{
		for(int i = 0; i < 6; i ++)
		{
			if(garbage_system->pitches[i]->motor_status == garbage_pitch::motor_status_t::MOTOR_STOP)
			{
				if(count >= 20)
				{
					xSemaphoreTake(uart_ttl_mutex, portMAX_DELAY);
					garbage_system->usart_send_data(UART3, (uint8_t []){0xfe, static_cast<uint8_t>(i + 1)}, 2);
					xSemaphoreGive(uart_ttl_mutex);
				}
				temp = adc_values[i];
				
				if(temp > garbage_system->pitches[i]->adc_zero + 100)
					count++;
				else
				{
					if(count)
						count--;
				}
			}
		}
		vTaskDelayUntil(&xLastWakeTime, delay_50ms);	
	}
}

void bsp_led(void *arg)
{
	while(1)
	{
		garbage_system->bsp_led_ctl(garbage::bsp_led_status_t::ON);
		vTaskDelay(300);
		garbage_system->bsp_led_ctl(garbage::bsp_led_status_t::OFF);
		vTaskDelay(100);
		garbage_system->bsp_led_ctl(garbage::bsp_led_status_t::ON);
		vTaskDelay(300);
		garbage_system->bsp_led_ctl(garbage::bsp_led_status_t::OFF);
		vTaskDelay(100);
		garbage_system->bsp_led_ctl(garbage::bsp_led_status_t::ON);
		vTaskDelay(500);
		garbage_system->bsp_led_ctl(garbage::bsp_led_status_t::OFF);
		vTaskDelay(500);
	}
}

void task_dog_feed(void *arg)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t xDelay10ms = pdMS_TO_TICKS(10);
	
	rcu_osci_on(RCU_IRC40K);
	while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K)){
	}
	
	fwdgt_write_enable();
	fwdgt_config(2500, FWDGT_PSC_DIV16);
	fwdgt_write_disable();
	fwdgt_enable();
	
	while(1)
	{
		fwdgt_write_enable();
		fwdgt_counter_reload();
		fwdgt_write_disable();
		vTaskDelayUntil(&xLastWakeTime, xDelay10ms);
	}
}

garbage::garbage()
{
	
}

extern "C" void systick_config();
void garbage::garbage_initialize()
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	__set_PRIMASK(1);

	delay_init();
	
	_74hc595_init();
	_74hc595_ctl(CHANNEL_1, VCC_5V, ON, 0);
	_74hc595_ctl(CHANNEL_2, VCC_5V, ON, 0);
	_74hc595_ctl(CHANNEL_3, VCC_5V, ON, 0);
	_74hc595_ctl(CHANNEL_4, VCC_5V, ON, 0);
	_74hc595_ctl(CHANNEL_5, VCC_5V, ON, 0);
	_74hc595_ctl(CHANNEL_6, VCC_5V, ON, 1);
	
	pitch_init();
	adc_init();
	adc_get_zero();
	
	bsp_led_init();
	usart_init(UART3, 115200);
	//uart_ttl_mutex = xSemaphoreCreateMutex();
	ModbusAppInit();
	
	for(int i = 0; i < 6; i++)
	{
		xTaskCreate(motor_task, "motor", 1024, (void *)i, 10, &motor_task_handle[i]);
	}
	
	xTaskCreate(modbus_task, "modbus", 128, NULL, 10, NULL);
//	xTaskCreate(garbage_detect_task, "detect", 128, NULL, 10, NULL);
//	xTaskCreate(date_current_detect, "date_detect", 128, NULL, 10, NULL);
	xTaskCreate(bsp_led, "led", 128, NULL, 10, NULL);
//	xTaskCreate(task_dog_feed, "dog_feed", 128, NULL, 10, NULL);
	
	vTaskStartScheduler();
}

void garbage::pitch_init()
{
	pitches.push_back(std::make_unique<garbage_pitch>(0,
												      ADC_CHANNEL_8,
													  MOTOR1_CTL1_PORT_RCU, MOTOR1_CTL2_PORT_RCU, MOTOR1_CTL_PORT1, MOTOR1_CTL_PORT2, MOTOR1_CTL_PIN1, MOTOR1_CTL_PIN2,
													  KEY_1_GPIO_RCU, KEY_1_GPIO, KEY_1_PIN,
													  FULL_LED_1_GPIO_RCU, FULL_LED_1_GPIO, FULL_LED_1_PIN));
	pitches.push_back(std::make_unique<garbage_pitch>(1,
												      ADC_CHANNEL_9,
													  MOTOR2_CTL1_PORT_RCU, MOTOR2_CTL2_PORT_RCU, MOTOR2_CTL_PORT1, MOTOR2_CTL_PORT2, MOTOR2_CTL_PIN1, MOTOR2_CTL_PIN2,
													  KEY_2_GPIO_RCU, KEY_2_GPIO, KEY_2_PIN,
													  FULL_LED_2_GPIO_RCU, FULL_LED_2_GPIO, FULL_LED_2_PIN));
	pitches.push_back(std::make_unique<garbage_pitch>(2,
												      ADC_CHANNEL_0,
													  MOTOR3_CTL1_PORT_RCU, MOTOR3_CTL2_PORT_RCU, MOTOR3_CTL_PORT1, MOTOR3_CTL_PORT2, MOTOR3_CTL_PIN1, MOTOR3_CTL_PIN2,
													  KEY_3_GPIO_RCU, KEY_3_GPIO, KEY_3_PIN,
													  FULL_LED_3_GPIO_RCU, FULL_LED_3_GPIO, FULL_LED_3_PIN));
	pitches.push_back(std::make_unique<garbage_pitch>(3,
												      ADC_CHANNEL_1,
													  MOTOR4_CTL1_PORT_RCU, MOTOR4_CTL2_PORT_RCU, MOTOR4_CTL_PORT1, MOTOR4_CTL_PORT2, MOTOR4_CTL_PIN1, MOTOR4_CTL_PIN2,
													  KEY_4_GPIO_RCU, KEY_4_GPIO, KEY_4_PIN,
													  FULL_LED_4_GPIO_RCU, FULL_LED_4_GPIO, FULL_LED_4_PIN));
	pitches.push_back(std::make_unique<garbage_pitch>(4,
												      ADC_CHANNEL_15,
													  MOTOR5_CTL1_PORT_RCU, MOTOR5_CTL2_PORT_RCU, MOTOR5_CTL_PORT1, MOTOR5_CTL_PORT2, MOTOR5_CTL_PIN1, MOTOR5_CTL_PIN2,
													  KEY_5_GPIO_RCU, KEY_5_GPIO, KEY_5_PIN,
													  FULL_LED_5_GPIO_RCU, FULL_LED_5_GPIO, FULL_LED_5_PIN));
	pitches.push_back(std::make_unique<garbage_pitch>(5,
												      ADC_CHANNEL_14,
													  MOTOR6_CTL1_PORT_RCU, MOTOR6_CTL2_PORT_RCU, MOTOR6_CTL_PORT1, MOTOR6_CTL_PORT2, MOTOR6_CTL_PIN1, MOTOR6_CTL_PIN2,
													  KEY_6_GPIO_RCU, KEY_6_GPIO, KEY_6_PIN,
													  FULL_LED_6_GPIO_RCU, FULL_LED_6_GPIO, FULL_LED_6_PIN));
}

void garbage::adc_init()
{
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_ADC0);
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
	
	dma_parameter_struct dma_config = {0};
	dma_deinit(DMA0, DMA_CH0);
	dma_config.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
	dma_config.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_config.memory_addr  = (uint32_t)adc_values;
	dma_config.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_config.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_config.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_config.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_config.number 		= 6;
	dma_config.priority     = DMA_PRIORITY_HIGH;
	dma_init(DMA0, DMA_CH0, &dma_config);
	dma_circulation_enable(DMA0, DMA_CH0);
	dma_channel_enable(DMA0, DMA_CH0);
	
	adc_deinit(ADC0);
	adc_mode_config(ADC_MODE_FREE);
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 6);
	for(int i = 0; i < 6; i ++)
		adc_regular_channel_config(ADC0, i, pitches[i]->adc_channel, ADC_SAMPLETIME_55POINT5);
	
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	adc_enable(ADC0);
	delay_nms(3);
	adc_calibration_enable(ADC0);
	adc_dma_mode_enable(ADC0);
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

void garbage::usart_init(uint32_t usart_no, uint32_t baudrate)
{
	if(usart_no == UART3)
	{
		rcu_periph_clock_enable(RCU_GPIOC);
		
		gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_11);
		gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
		
		rcu_periph_clock_enable(RCU_UART3);
		usart_deinit(UART3);
		usart_baudrate_set(UART3, baudrate);
		usart_word_length_set(UART3, USART_WL_8BIT);
		usart_stop_bit_set(UART3, USART_STB_1BIT);
		usart_parity_config(UART3, USART_PM_NONE);
		usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
		usart_receive_config(UART3, USART_RECEIVE_ENABLE);

		usart_enable(UART3);
	}
}

void garbage::usart_send_data(uint32_t usart_no, uint8_t *buf, uint8_t len)
{
	while(len--)
	{
		usart_data_transmit(usart_no, *buf++);
		while(RESET == usart_flag_get(usart_no, USART_FLAG_TC));
	}
}

void garbage::adc_get_zero()
{
	for(int i = 0; i < 500; i ++)
	{
		for(int j = 0; j < 6; j ++)
		{
			pitches[j]->adc_zero += adc_values[j];
		}
	}
	for(int i = 0; i < 6; i++)
	{
		pitches[i]->adc_zero /= 500;
	}
}

void garbage::bsp_led_init()
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_2MHZ ,GPIO_PIN_15);
}

void garbage::bsp_led_ctl(bsp_led_status_t bsp_led_statut)
{
	if(bsp_led_statut == bsp_led_status_t::ON)
	{
		gpio_bit_set(GPIOA, GPIO_PIN_15);
	}
	else
	{
		gpio_bit_reset(GPIOA, GPIO_PIN_15);
	}
}


extern "C" void Main(void)
{
	garbage_system = garbage::creat_garbage();
	garbage_system->garbage_initialize();
}

extern "C" uint16_t Read_channel_init(void)
{
	uint16_t temp = 0;
	for(int i = 0; i < 6; i++)
	{
		if(garbage_system->pitches[i]->current_init_flag)
			temp |= 1 << i;
		else
			temp &= ~(1 << i);
	}
	
	return temp;
}

extern "C" uint16_t Read_error_state(void)
{
	uint16_t temp = 0;
	for(int i = 0; i < 6; i++)
	{
		if(garbage_system->pitches[i]->pitch_error_flag)
			temp |= 1 << i;
		else
			temp &= ~(1 << i);
	}
	
	return temp;
}

extern "C" void write_pitch_work(int pitch, uint16_t val)
{
	switch(val)
	{
		case 4:
			xTaskNotify(motor_task_handle[pitch], DOOR_OPEN_START, eSetBits);
			break;
		case 5:
			xTaskNotify(motor_task_handle[pitch], DOOR_CLOSE_START, eSetBits);
			break;
		case 6:
			garbage_system->pitches[pitch]->motor_ctl(garbage_pitch::motor_cmd_t::STOP);
			break;
		case 7:
			garbage_system->pitches[pitch]->full_light_ctl(garbage_pitch::full_light_type_t::RED_LED, 0,0,0, garbage_pitch::full_light_status_t::OFF);
			break;
		case 8:
			garbage_system->pitches[pitch]->full_light_ctl(garbage_pitch::full_light_type_t::RED_LED, 0,0,0, garbage_pitch::full_light_status_t::ON);
			break;
		case 9:
			xTaskNotify(motor_task_handle[pitch], DOOR_GET_CURRENT, eSetBits);
			break;
		default:
			break;
	}
}

extern "C" void Write_led_r(int pitch, uint16_t val)
{
	garbage_system->pitches[pitch]->full_light.r = val;
}

extern "C" void Write_led_g(int pitch, uint16_t val)
{
	garbage_system->pitches[pitch]->full_light.g = val;
}

extern "C" void Write_led_b(int pitch, uint16_t val)
{
	garbage_system->pitches[pitch]->full_light.b = val;
}

extern "C" void write_channel_init_flag(uint16_t val)
{
	switch(val)
	{
		case 1:
			garbage_system->pitches[0]->current_init_flag = 0;	
			break;
		case 2:
			garbage_system->pitches[1]->current_init_flag = 0;	
			break;
		case 3:
			garbage_system->pitches[2]->current_init_flag = 0;	
			break;
		case 4:
			garbage_system->pitches[3]->current_init_flag = 0;	
			break;
		case 5:
			garbage_system->pitches[4]->current_init_flag = 0;	
			break;
		case 6:
			garbage_system->pitches[5]->current_init_flag = 0;	
			break;
		default:
			break;
	}
}





