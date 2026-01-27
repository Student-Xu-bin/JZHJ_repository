#include "app.h"
#include "delay.h"

void stepper_find_zero(channel_t *channel)
{
	#if 1
	//FlagStatus ret;
	stepper_t *stepper = &channel->autochange.stepper;
	channel->autochange.Zero_back = zero_task;
	if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == SET)
	{
		auto_change_reverse(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin);
		channel->autochange.zero_dir_flag = SET;
	}
	else
		auto_change_forward(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin); //方向正
	timer_autoreload_value_config(stepper->stepper_hw_info.timer_periph, 1000);
	timer_enable(stepper->stepper_hw_info.timer_periph);
	vTaskSuspend(channel->task_handle);
	//xSemaphoreGive(stepper->idle_semaphore_handle);
	//while(!channel->autochange.Zero_back);
	//while(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET);
	#if 0
	for(int i = 0; i < 10000; i++)
	{
		if(gpio_input_bit_get(GPIOE, GPIO_PIN_3) == RESET)
			gpio_bit_set(GPIOE, GPIO_PIN_3);
		else
			gpio_bit_reset(GPIOE, GPIO_PIN_3);
		//delay_nus(100);
	}
	#endif
//	while(1);
	//delay_nms(10);
	//taskENTER_CRITICAL();
	
	//taskEXIT_CRITICAL();
	
//	while(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET);
//	timer_disable(stepper->stepper_hw_info.timer_periph);
//	timer_counter_value_config(stepper->stepper_hw_info.timer_periph, 0);
//	channel->autochange.Zero_back = no_zero_task;
	#endif
#if 0
	stepper_t *stepper = &channel->autochange.stepper;
	channel->autochange.Zero_back = zero_task;
//	auto_change_forward(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin); //方向正
//	if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
//	{
//		stepper_work(&channel->autochange, FORWARD, 0.5);
//		WAIT_FOR_STEPPER(channel);
//		while(cd4051_detect(channel->no, STEPPER_ZERO_DET) == SET)
//		{
//			timer_disable(stepper->stepper_hw_info.timer_periph);
//			auto_change_reverse(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin);
//		}
//	}	
	uint8_t flag = 0;
	FlagStatus ret;
	while(ret == SET)
	{
		flag = 1;
		auto_change_reverse(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin); //方向反
		if(gpio_input_bit_get(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin) == RESET)
			gpio_bit_set(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin);
		else
			gpio_bit_reset(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin);
		delay_nus(200);
		taskENTER_CRITICAL();
		ret = cd4051_detect(channel->no, STEPPER_ZERO_DET);
		taskEXIT_CRITICAL();
	}
	if(flag)
	{
		timer_enable(stepper->stepper_hw_info.timer_periph);
	}
	while(ret == RESET)
	{
		auto_change_reverse(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin); //方向正
		
		if(gpio_input_bit_get(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin) == RESET)
			gpio_bit_set(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin);
		else
			gpio_bit_reset(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin);
		delay_nus(200);
		taskENTER_CRITICAL();
		ret = cd4051_detect(channel->no, STEPPER_ZERO_DET);
		taskEXIT_CRITICAL();
	}
	timer_disable(stepper->stepper_hw_info.timer_periph);
	timer_counter_value_config(stepper->stepper_hw_info.timer_periph, 0);
	channel->autochange.Zero_back = no_zero_task;
	
//	for(int i = 0; i < 1000; i ++)
//	{
//		if(gpio_input_bit_get(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin) == RESET)
//			gpio_bit_set(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin);
//		else
//			gpio_bit_reset(stepper->stepper_hw_info.pul_gpio, stepper->stepper_hw_info.pul_pin);
//		delay_nus(200);
//	}
#endif
}

void channel_full_detect(channel_t *channel)
{
	uint32_t steps_sum_tmp = 0;
	float    distance_sum_tmp = 0.0;
	
	taskENTER_CRITICAL();
	FlagStatus ret = cd4051_detect(channel->no, STEPPER_ZERO_DET);
	taskEXIT_CRITICAL();
	
	if(ret == RESET)		// 不在零点直接返回
		return;
	
	for(int i = 0; i < DUSTBIN_NUMBERS; i ++)
	{
		float tmp = channel->autochange.f_dustbin_postion[i] - channel->autochange.f_full_detect_distance - distance_sum_tmp;
		if(tmp <= 0)
			tmp = 0;
		stepper_work(&channel->autochange, FANXIANG, tmp);
		WAIT_FOR_STEPPER(channel);
		
		distance_sum_tmp += tmp;
		steps_sum_tmp += channel->autochange.stepper.step_count;
		
		taskENTER_CRITICAL();
		ret = cd4051_detect(channel->no, IR_DET);
		taskEXIT_CRITICAL();
		
		if(ret == SET)
			channel->autochange.full_state |= (1 << i);
		else
			channel->autochange.full_state &= ~(1 << i);
	}
	
	vTaskDelay(800);
	stepper_set(&channel->autochange.stepper, FORWARD, steps_sum_tmp, 1);
	WAIT_FOR_STEPPER(channel);
}

void full_light_show(channel_t *channel)
{
	uint8_t sum_tmp = 0;
	
	for(int i = 0; i < DUSTBIN_NUMBERS; i ++)
		if(channel->autochange.full_state & (1 << i))
			sum_tmp += 1;
		
	switch(sum_tmp)
	{
		case 0 :
			full_light_ctl(&channel->door.full_light, 0xff, 0x00, 0x00);
			break;
		case 1 :
			full_light_ctl(&channel->door.full_light, 0x00, 0x00, 0xff);
			break;
		case 2 :
			full_light_ctl(&channel->door.full_light, 0xff, 0xff, 0x00);
			break;
		case 3 :
			full_light_ctl(&channel->door.full_light, 0x00, 0xff, 0x00);
			break;
		case 4 :
			full_light_ctl(&channel->door.full_light, 3, 255, 244);
			break;
		default :
			break;
	}
}

static void throw_full_detect(channel_t *channel, uint32_t which)
{
	taskENTER_CRITICAL();
	FlagStatus ret = cd4051_detect(channel->no, IR_DET);
	taskEXIT_CRITICAL();
	
	if(which == 1)
		which = 0;
	else if(which == 2)
		which = 1;
	else if(which == 3)
		which = 2;
	
	if(ret == SET)
		channel->autochange.full_state |= (1 << which);
	else
		channel->autochange.full_state &= ~(1 << which);
}

void throw_garbage(channel_t *channel, uint32_t which)
{
	float disc_tmp = 0.0;
	
	taskENTER_CRITICAL();
	FlagStatus ret = cd4051_detect(channel->no, STEPPER_ZERO_DET);
	taskEXIT_CRITICAL();
	
	if(ret == RESET)
		return;
	
	if(which == STEPPER_THROW_1)
	{
		disc_tmp = channel->autochange.f_dustbin_postion[0];
		which = 1;
	}
	else if(which == STEPPER_THROW_2)
	{
		disc_tmp = channel->autochange.f_dustbin_postion[1];
		which = 2;
	}
	else if(which == STEPPER_THROW_3)
	{
		disc_tmp = channel->autochange.f_dustbin_postion[2];
		which = 3;
	}
//	else if(which == STEPPER_THROW_4)
//	{
//		disc_tmp = channel->autochange.f_dustbin_postion[3];
//		which = 4;
//	}
	else 	// 自动找桶
	{
//		if(!(channel->autochange.full_state & (1 << 3)))
//		{
//			disc_tmp = channel->autochange.f_dustbin_postion[3];
//			which = 4;
//		}
		if(!(channel->autochange.full_state & (1 << 2)))
		{
			disc_tmp = channel->autochange.f_dustbin_postion[2];
			which = 3;
		}
		else if(!(channel->autochange.full_state & (1 << 1)))
		{
			disc_tmp = channel->autochange.f_dustbin_postion[1];
			which = 2;
		}
		else if(!(channel->autochange.full_state & (1 << 0)))
		{
			disc_tmp = channel->autochange.f_dustbin_postion[0];
			which = 1;
		}
	}
	
	stepper_work(&channel->autochange, FANXIANG, disc_tmp);
	
	WAIT_FOR_STEPPER(channel); // 到位
	if(channel->bags_broken != NULL && channel->bags_broken != (void *)-1)
		bags_broken_over(channel->bags_broken);
	vTaskDelay(600);
	
	linear_actuator_ctl(&channel->autochange.linear_actuator, LA_OPEN);
	vTaskDelay(3000);
	linear_actuator_ctl(&channel->autochange.linear_actuator, LA_CLOSE);
	vTaskDelay(3000);
	
	uint32_t steps_sum_tmp = channel->autochange.stepper.step_count;
	
	if(which == 1) // 第一个桶
	{
		stepper_work(&channel->autochange, FORWARD, channel->autochange.f_dustbin_postion[0]);
	}
	else
	{
		stepper_work(&channel->autochange, FORWARD, channel->autochange.f_full_detect_distance);
	}
	WAIT_FOR_STEPPER(channel);
	
	throw_full_detect(channel, which);	// 满桶检测
	taskENTER_CRITICAL();
	full_light_show(channel);
	taskEXIT_CRITICAL();
	
	if(which == 1)
		return;
	
	steps_sum_tmp -= channel->autochange.stepper.step_count;
	
	//stepper_work(&channel->autochange, FORWARD, disc_tmp - channel->autochange.f_full_detect_distance);
	stepper_set(&channel->autochange.stepper, FORWARD, steps_sum_tmp, 1);	// 回零
	WAIT_FOR_STEPPER(channel);
}

void gs_detect(channel_t *channel)
{
	static enum linear_actuator_state_enum_t old_inear_actuator_state;
	
	taskENTER_CRITICAL();
	
	if(channel->door.linear_actuator.state != LA_STOP)
	{
		old_inear_actuator_state = channel->door.linear_actuator.state;
	}
	
	
	if(cd4051_detect(channel->no, GS_DET) == SET)
	{
		linear_actuator_ctl(&channel->door.linear_actuator, LA_STOP);
	}
	else
	{
		linear_actuator_ctl(&channel->door.linear_actuator, old_inear_actuator_state);
	}
	
	taskEXIT_CRITICAL();
}


/* "CHx DOOR KEY ON" */

static uint32_t door_key_count[6] = {0};
static uint32_t green_key_count = 0;
static uint32_t red_key_count = 0;

void key_detect(channel_t *channel)
{
	
	uint8_t buffer[]  = {0, 0xff};//"CHx DOOR KEY ON\r\n"; // 门板按钮按下时发送 eg: CH1 DOOR KEY ON\r\n
	uint8_t buffer2[] = {7, 0xff};//"CHANGE GREEN ON\r\n"; // 人工换桶绿色按钮按下时发送
	uint8_t buffer3[] = {8, 0xff};//"CHANGE RED ON\r\n";   // 人工换桶红色自锁按钮按下时发送
	uint8_t buffer4[] = {9, 0xff};//"CHANGE RED OFF\r\n";  // 人工换桶红色自锁按钮松开时发送
	
	
	//FlagStatus ret;
	
	taskENTER_CRITICAL();
	buffer[0] = (uint8_t)channel->no;
//	ret = cd4051_detect(channel->no, KEY_DET);
//	

//	taskEXIT_CRITICAL();
	
	//if(ret == SET)
	if(cd4051_detect(channel->no, KEY_DET) == SET)
	{	
		
		//while(cd4051_detect(channel->no, KEY_DET) == SET);
		door_key_count[channel->no] += 1;
		if(door_key_count[channel->no] == 1)
		{
			for(int i = 0; i < sizeof(buffer); i += 1)
			{
				while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
				usart_data_transmit(USART1, buffer[i]);
			}
		}
	}
	else
	{
		//taskEXIT_CRITICAL();
		door_key_count[channel->no] = 0;
	}
	taskEXIT_CRITICAL();
	
	if(channel->no == CHANNEL_1) // 只需第一个通道检测即可
	{
		if(man_key_detect(MAN_KEY_GREEN) == SET)
		{
			green_key_count += 1;
			
			if(green_key_count == 1)
			{
				for(int i = 0; i < sizeof(buffer2); i += 1)
				{
					while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
					usart_data_transmit(USART1, buffer2[i]);
				}
			}
		}
		else
		{
			green_key_count = 0;
		}
		
		if(man_key_detect(MAN_KEY_RED) == SET && red_key_count == 0)
		{
			red_key_count += 1;
			
			for(int i = 0; i < sizeof(buffer3); i += 1)
			{
				while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
				usart_data_transmit(USART1, buffer3[i]);
			}
		}
		else if(man_key_detect(MAN_KEY_RED) == RESET && red_key_count != 0)
		{
			red_key_count = 0;
			
			for(int i = 0; i < sizeof(buffer4); i += 1)
			{
				while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
				usart_data_transmit(USART1, buffer4[i]);
			}
		}
	}	
}

void proximity_switch_detect(channel_t *channel)
{
	enum linear_actuator_state_enum_t ret;
	
	taskENTER_CRITICAL();
	
	ret = channel->door.linear_actuator.state;
	
	switch(ret)
	{
	case LA_OPEN :
		if(cd4051_detect(channel->no, DOOR_OPEN_DET) == SET)
		{
			linear_actuator_ctl(&channel->autochange.linear_actuator, LA_STOP);//xTaskNotify(channel->task_handle, DOOR_STOP, eSetBits);
			channel->door.ps_detect_flag = PROXIMITY_SWITCH_OFF;
			full_light_show(channel);
		}
		break;
	case LA_CLOSE :
		if(cd4051_detect(channel->no, DOOR_CLOSE_DET) == SET)
		{
			linear_actuator_ctl(&channel->autochange.linear_actuator, LA_STOP);
			//xTaskNotify(channel->task_handle, DOOR_STOP, eSetBits);
			channel->door.ps_detect_flag = PROXIMITY_SWITCH_OFF;
			full_light_show(channel);
		}
		break;
	default :
		break;
	}
	
	taskEXIT_CRITICAL();
}

void door_key_led(channel_t *channel)
{
	taskENTER_CRITICAL();
	
	if((cd4051_detect(channel->no, STEPPER_ZERO_DET) == SET) && (cd4051_detect(channel->no, DOOR_CLOSE_DET) == SET))
	{
		#if 1
		if(channel->bags_broken != NULL && channel->bags_broken!= (void *)-1)
		{
			if(channel->bags_broken->state == BAGS_BROKEN_IDLE)
			{
				_74hc595_ctl(channel->no, KEY_LED, ON, 1);
				taskEXIT_CRITICAL();
				return;
			}
			else
			{
				_74hc595_ctl(channel->no, KEY_LED, OFF, 1);
				taskEXIT_CRITICAL();
				return;
			}
		}
		#endif
		_74hc595_ctl(channel->no, KEY_LED, ON, 1);
	}
	else
	{
		_74hc595_ctl(channel->no, KEY_LED, OFF, 1);
	}

	taskEXIT_CRITICAL();
}

static uint8_t alm_message[] = {0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6};

void stepper_alm_detect(channel_t *channel)
{
	FlagStatus ret = SET;
	ret = cd4051_detect((enum channel_enum_t)channel->no, STEPPER_ALM_DET);
	taskENTER_CRITICAL();
	if(ret == RESET){
		while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
		usart_data_transmit(USART1, alm_message[channel->no]);
		while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
		usart_data_transmit(USART1, 0xff);
	}
	taskEXIT_CRITICAL();
}
