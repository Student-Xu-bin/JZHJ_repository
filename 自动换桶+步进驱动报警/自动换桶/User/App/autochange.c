#include "autochange.h"
#include "CD4051.h"
#include "channel.h"
#include "app.h"

extern uint16_t g_acc_stepper_S[];

static autochange_t *autochange[6] = {0};

int autochange_init(autochange_t *a, uint8_t no)
{
	if(!a || no > 5)
		return -1;

	autochange[no] 				= a;
	a->full_state 				= 0x00;
	a->f_dustbin_postion[0] 	= DUSTBIN1_DISTANCE;
	a->f_dustbin_postion[1] 	= DUSTBIN2_DISTANCE;
	a->f_dustbin_postion[2] 	= DUSTBIN3_DISTANCE;
	//a->f_dustbin_postion[3]     = DUSTBIN4_DISTANCE;
	a->f_total_distance 		= DEFAULT_TOTAL_DISTANCE;
	a->f_full_detect_distance 	= FULL_DETECT_DISTANCE;
	a->weight               	= 0;
	a->Zero_back 				= no_zero_task;
	a->zero_dir_flag            = RESET;
	linear_actuator_init(&a->linear_actuator);
	stepper_init(&a->stepper);
	
	return 0;
}

//速度调节函数
static void stepper_set_frequency(stepper_t *stepper)
{
	BaseType_t pd = pdFALSE;
	
	int32_t tmp;
	stepper->step_count ++;
	
    switch(stepper->status)
    {  
		case STEPPER_INCREASE://加速阶段
			if(stepper->step_count < stepper->acc_step_max)
			{
				timer_autoreload_value_config(stepper->stepper_hw_info.timer_periph, g_acc_stepper_S[stepper->step_count]);
			}
			else
			{
				stepper->status = STEPPER_STABLE;
			}
			break;
		case STEPPER_STABLE://恒速阶段
			if(stepper->target_step - stepper->step_count <= stepper->acc_step_max)
			{
				stepper->status = STEPPER_DECREASE;
			}
			break;
		case STEPPER_DECREASE://减速阶段
			if(stepper->step_count >= stepper->target_step)
			{
				stepper->status = STEPPER_IDLE;
			}
			else
			{
				tmp = stepper->target_step - stepper->step_count;
				if(tmp >= 0)
					timer_autoreload_value_config(stepper->stepper_hw_info.timer_periph, g_acc_stepper_S[tmp]);//计算下一个PWM的周期
				break;
			}
		case STEPPER_IDLE ://空间阶段，即已经到达目标位
			timer_disable(stepper->stepper_hw_info.timer_periph);
			timer_counter_value_config(stepper->stepper_hw_info.timer_periph, 0);
		
			xSemaphoreGiveFromISR(stepper->idle_semaphore_handle, &pd);//步进电机到位增加信号量
			
			break;
		default :
			break;
    }
}
#if 1
void STEPPER1_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER1_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		
		if((autochange[0]->Zero_back) && (zero_task && autochange[0]->zero_dir_flag == RESET))
		{
			if(gpio_input_bit_get(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
			else
				gpio_bit_set(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
			if(cd4051_detect(CHANNEL_1, STEPPER_ZERO_DET) == SET)
			{
				static uint32_t count = 0;
				if(++count > 500)
				{
					timer_disable(autochange[0]->stepper.stepper_hw_info.timer_periph);
					autochange[0]->Zero_back = no_zero_task;
					count = 0;
					xTaskResumeFromISR(channel[0].task_handle);
				}
			}
		}
		else if((autochange[0]->Zero_back) && (zero_task && autochange[0]->zero_dir_flag == SET))
		{
			if(gpio_input_bit_get(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
			else
				gpio_bit_set(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
			if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
			{
				static uint32_t delay_count = 0;
				
				if((++delay_count >1000) && (cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET))
				{
					autochange[0]->zero_dir_flag = RESET;
					auto_change_forward(STEPPER1_DIR_PORT, STEPPER1_DIR_PIN);
				}
			}
		}
		else
		{
			gpio_bit_set(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
			
			stepper_set_frequency(&autochange[0]->stepper);
			
			gpio_bit_reset(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
		}		
	
		timer_interrupt_flag_clear(STEPPER1_TIMER, TIMER_INT_FLAG_UP);
	}
}

void STEPPER2_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER2_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		
		if((autochange[1]->Zero_back) && (zero_task && autochange[1]->zero_dir_flag == RESET))
		{
			if(gpio_input_bit_get(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
			else
				gpio_bit_set(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
			if(cd4051_detect(CHANNEL_2, STEPPER_ZERO_DET) == SET)
			{
				static uint32_t count = 0;
				if(++count > 500)
				{
					timer_disable(autochange[1]->stepper.stepper_hw_info.timer_periph);
					autochange[1]->Zero_back = no_zero_task;
					count = 0;
					xTaskResumeFromISR(channel[1].task_handle);
				}
			}
		}
		else if((autochange[1]->Zero_back) && (zero_task && autochange[1]->zero_dir_flag == SET))
		{
			if(gpio_input_bit_get(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
			else
				gpio_bit_set(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
			if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
			{
				static uint32_t delay_count = 0;
				
				if((++delay_count >1000) && (cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET))
				{
					autochange[1]->zero_dir_flag = RESET;
					auto_change_forward(STEPPER2_DIR_PORT, STEPPER2_DIR_PIN);
				}
			}
		}
		else
		{
			gpio_bit_set(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
		
			stepper_set_frequency(&autochange[1]->stepper);
		
			gpio_bit_reset(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
		}
		timer_interrupt_flag_clear(STEPPER2_TIMER, TIMER_INT_FLAG_UP);
	}
}

void STEPPER3_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER3_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		if((autochange[2]->Zero_back) && (zero_task && autochange[2]->zero_dir_flag == RESET))
		{
			if(gpio_input_bit_get(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
			else
				gpio_bit_set(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
			if(cd4051_detect(CHANNEL_3, STEPPER_ZERO_DET) == SET)
			{
				static uint32_t count = 0;
				if(++count > 500)
				{
					timer_disable(autochange[2]->stepper.stepper_hw_info.timer_periph);
					autochange[2]->Zero_back = no_zero_task;
					count = 0;
					xTaskResumeFromISR(channel[2].task_handle);
				}
			}
		}
		else if((autochange[2]->Zero_back) && (zero_task && autochange[2]->zero_dir_flag == SET))
		{
			if(gpio_input_bit_get(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
			else
				gpio_bit_set(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
			if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
			{
				static uint32_t delay_count = 0;
				
				if((++delay_count >1000) && (cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET))
				{
					autochange[2]->zero_dir_flag = RESET;
					auto_change_forward(STEPPER3_DIR_PORT, STEPPER3_DIR_PIN);
				}
			}
		}
		else
		{
			gpio_bit_set(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
		
			stepper_set_frequency(&autochange[2]->stepper);
		
			gpio_bit_reset(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
		}
		
		timer_interrupt_flag_clear(STEPPER3_TIMER, TIMER_INT_FLAG_UP);
	}
}

void STEPPER4_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER4_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		if((autochange[3]->Zero_back) && (zero_task && autochange[3]->zero_dir_flag == RESET))
		{
			if(gpio_input_bit_get(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
			else
				gpio_bit_set(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
			if(cd4051_detect(CHANNEL_4, STEPPER_ZERO_DET) == SET)
			{
				static uint32_t count = 0;
				if(++count > 500)
				{
					timer_disable(autochange[3]->stepper.stepper_hw_info.timer_periph);
					autochange[3]->Zero_back = no_zero_task;
					count = 0;
					xTaskResumeFromISR(channel[3].task_handle);
				}
			}
		}
		else if((autochange[3]->Zero_back) && (zero_task && autochange[3]->zero_dir_flag == SET))
		{
			if(gpio_input_bit_get(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
			else
				gpio_bit_set(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
			if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
			{
				static uint32_t delay_count = 0;
				
				if((++delay_count >1000) && (cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET))
				{
					autochange[3]->zero_dir_flag = RESET;
					auto_change_forward(STEPPER4_DIR_PORT, STEPPER4_DIR_PIN);
				}
			}
		}
		else
		{
			gpio_bit_set(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
		
			stepper_set_frequency(&autochange[3]->stepper);
		
			gpio_bit_reset(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
		}
		
		timer_interrupt_flag_clear(STEPPER4_TIMER, TIMER_INT_FLAG_UP);
	}
}

void STEPPER5_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER5_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		if((autochange[4]->Zero_back) && (zero_task && autochange[4]->zero_dir_flag == RESET))
		{
			if(gpio_input_bit_get(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
			else
				gpio_bit_set(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
			if(cd4051_detect(CHANNEL_5, STEPPER_ZERO_DET) == SET)
			{
				static uint32_t count = 0;
				if(++count > 500)
				{
					timer_disable(autochange[4]->stepper.stepper_hw_info.timer_periph);
					autochange[4]->Zero_back = no_zero_task;
					count = 0;
					xTaskResumeFromISR(channel[4].task_handle);
				}
			}
		}
		else if((autochange[4]->Zero_back) && (zero_task && autochange[4]->zero_dir_flag == SET))
		{
			if(gpio_input_bit_get(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
			else
				gpio_bit_set(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
			if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
			{
				static uint32_t delay_count = 0;
				
				if((++delay_count >1000) && (cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET))
				{
					autochange[4]->zero_dir_flag = RESET;
					auto_change_forward(STEPPER5_DIR_PORT, STEPPER5_DIR_PIN);
				}
			}
		}
		else
		{
			gpio_bit_set(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
		
			stepper_set_frequency(&autochange[4]->stepper);
		
			gpio_bit_reset(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
		}
		
		timer_interrupt_flag_clear(STEPPER5_TIMER, TIMER_INT_FLAG_UP);
	}
}

void STEPPER6_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER6_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		if((autochange[5]->Zero_back) && (zero_task && autochange[5]->zero_dir_flag == RESET))
		{
			if(gpio_input_bit_get(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
			else
				gpio_bit_set(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
			if(cd4051_detect(CHANNEL_6, STEPPER_ZERO_DET) == SET)
			{
				static uint32_t count = 0;
				if(++count > 500)
				{
					timer_disable(autochange[5]->stepper.stepper_hw_info.timer_periph);
					autochange[5]->Zero_back = no_zero_task;
					count = 0;
					xTaskResumeFromISR(channel[5].task_handle);
				}
			}
		}
		else if((autochange[5]->Zero_back) && (zero_task && autochange[5]->zero_dir_flag == SET))
		{
			if(gpio_input_bit_get(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN) == SET)
				gpio_bit_reset(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
			else
				gpio_bit_set(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
			if(cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET)
			{
				static uint32_t delay_count = 0;
				
				if((++delay_count >1000) && (cd4051_detect(channel->no, STEPPER_ZERO_DET) == RESET))
				{
					autochange[5]->zero_dir_flag = RESET;
					auto_change_forward(STEPPER6_DIR_PORT, STEPPER6_DIR_PIN);
				}
			}
		}
		else
		{
			gpio_bit_set(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
		
			stepper_set_frequency(&autochange[5]->stepper);
		
			gpio_bit_reset(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
		}
		timer_interrupt_flag_clear(STEPPER6_TIMER, TIMER_INT_FLAG_UP);
	}
}
#endif

void stepper_set(stepper_t *stepper, FlagStatus s, uint32_t disc, uint8_t flag)
{
	if(flag == 1)
		stepper->acc_step_max = STEPPER_ACC_MAX;
	stepper->target_step = disc;
	stepper->step_count = 0;
	stepper->status = STEPPER_INCREASE;
	if(s == FANXIANG)
		auto_change_reverse(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin);//方向反
	else if(s == FORWARD)
		auto_change_forward(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin);//方向反
	timer_enable(stepper->stepper_hw_info.timer_periph);
}

void stepper_work(autochange_t *autochange, FlagStatus s, float disc)
{
#if 0
	if(!ptr->zero_flag)//步进电机没有在0位，不工作，直接返回
	{
		xSemaphoreGive(door->stepperMutex);//释放互斥锁
		return;
	}
#endif
	stepper_t *stepper = &autochange->stepper;
	
	uint32_t tmp_disc;
	
	if(disc > 0)
	{
		tmp_disc = disc / EACH_LAP * (float)STEPPER_SUBDIVISION;
		if(tmp_disc >= FULL_ACC_MAX)
			stepper->acc_step_max = STEPPER_ACC_MAX;
		else
			stepper->acc_step_max = (tmp_disc / 3);
	}
	else if(disc == 0)
	{
		stepper->step_count = 0;
		xSemaphoreGive(stepper->idle_semaphore_handle);//步进电机到位增加信号量
		return;
	}
#if 0
	else//自动找桶位
	{
		if(!(autochange->full_state & (1 << 2)))
			tmp_disc = (uint32_t)(autochange->f_dustbin_postion[DUSTBIN_3] / EACH_LAP * (float)STEPPER_SUBDIVISION);
		else if(!(autochange->full_state & (1 << 1)))
			tmp_disc = (uint32_t)(autochange->f_dustbin_postion[DUSTBIN_2] / EACH_LAP * (float)STEPPER_SUBDIVISION);
		else if(!(autochange->full_state & (1 << 0)))
			tmp_disc = (uint32_t)(autochange->f_dustbin_postion[DUSTBIN_1] / EACH_LAP * (float)STEPPER_SUBDIVISION);
		else
			return;
	}
#endif	
	stepper_set(stepper, s, tmp_disc, 0);
}
