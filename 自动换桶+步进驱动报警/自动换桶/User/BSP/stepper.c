#include "stepper.h"
#include "delay.h"
#include <math.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "create_task.h"
#include "door.h"

uint16_t g_acc_stepper_S[STEPPER_ACC_MAX] = {0};

static void stepper_timer_init(stepper_t *sp, uint32_t periodUs)
{
	rcu_periph_clock_enable(sp->stepper_hw_info.timer_rcu);
	
	timer_deinit(sp->stepper_hw_info.timer_periph);
	
	timer_parameter_struct timerInitPara;
	timer_struct_para_init(&timerInitPara);

	timerInitPara.prescaler = 120 - 1;

	timerInitPara.period = periodUs - 1;
	timer_init(sp->stepper_hw_info.timer_periph, &timerInitPara);
	timer_interrupt_flag_clear(sp->stepper_hw_info.timer_periph, TIMER_INT_FLAG_UP);
	
	timer_interrupt_enable(sp->stepper_hw_info.timer_periph, TIMER_INT_UP);

	nvic_irq_enable(sp->stepper_hw_info.timer_irq, 6, 0);

	timer_disable(sp->stepper_hw_info.timer_periph);
}

static void stepper_gpio_init(stepper_t *sp)
{
	rcu_periph_clock_enable(sp->stepper_hw_info.dir_gpio_rcu);
	if(sp->stepper_hw_info.pul_gpio_rcu != sp->stepper_hw_info.dir_gpio_rcu)
		rcu_periph_clock_enable(sp->stepper_hw_info.pul_gpio_rcu);

    gpio_bit_reset(sp->stepper_hw_info.pul_gpio, sp->stepper_hw_info.pul_pin);
    gpio_bit_set(sp->stepper_hw_info.dir_gpio, sp->stepper_hw_info.dir_pin);//初始化正方向

    gpio_init(sp->stepper_hw_info.pul_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, sp->stepper_hw_info.pul_pin);
    gpio_init(sp->stepper_hw_info.dir_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, sp->stepper_hw_info.dir_pin);
}

void stepper_init(stepper_t *sp)
{
	if(!sp)
		return;
	
    stepper_gpio_init(sp);
	stepper_timer_init(sp, 60000);
	
	sp->idle_semaphore_handle = xSemaphoreCreateBinary();					// 创建信号量
	
	timer_autoreload_value_config(sp->stepper_hw_info.timer_periph, g_acc_stepper_S[0]);
}

//功能：S加速曲线初始化
//参数1 *pbuff         计算出的定时器的周期
//参数2 fre_max        最大频率 Hz
//参数3 fre_min        最小频率 Hz
//参数4 len            加速需要的脉冲数
void stepper_calculate_S(uint16_t *pbuff, uint16_t fre_max, uint16_t fre_min, int16_t len)
{
	if(fre_min >= fre_max)
		return;
	
	uint16_t delta = fre_max - fre_min;
	float x = -2.0;
	
	for(int i = 0; i < STEPPER_ACC_MAX; i ++, x += (10.0f / (float)STEPPER_ACC_MAX))//0.00208f)
	{
		pbuff[i] = (uint16_t)((1.0 - (1.0 / (1.0 + exp(-x/2.2f)))) * delta) + fre_min;
		//pbuff[i] = (uint16_t)((1.0f - (1.0f / (1.0f + exp(-x/1.0f)))) * delta) + fre_min;
	}
}

#if 0
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

#include "autochange.h"
extern autochange_t autochange[];

void STEPPER1_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER1_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		gpio_bit_set(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
		
		stepper_set_frequency(&autochange[0].stepper);
		
		gpio_bit_reset(STEPPER1_PUL_PORT, STEPPER1_PUL_PIN);
		
		timer_interrupt_flag_clear(STEPPER1_TIMER, TIMER_INT_FLAG_UP);
	}
}

void STEPPER2_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER2_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(STEPPER2_TIMER, TIMER_INT_FLAG_UP);
		
		gpio_bit_set(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
		
		stepper_set_frequency(&autochange[1].stepper);
		
		gpio_bit_reset(STEPPER2_PUL_PORT, STEPPER2_PUL_PIN);
	}
}

void STEPPER3_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER3_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(STEPPER3_TIMER, TIMER_INT_FLAG_UP);
		
		gpio_bit_set(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
		
		stepper_set_frequency(&autochange[2].stepper);
		
		gpio_bit_reset(STEPPER3_PUL_PORT, STEPPER3_PUL_PIN);
	}
}

void STEPPER4_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER4_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(STEPPER4_TIMER, TIMER_INT_FLAG_UP);
		
		gpio_bit_set(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
		
		stepper_set_frequency(&autochange[3].stepper);
		
		gpio_bit_reset(STEPPER4_PUL_PORT, STEPPER4_PUL_PIN);
	}
}

void STEPPER5_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER5_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(STEPPER5_TIMER, TIMER_INT_FLAG_UP);
		
		gpio_bit_set(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
		
		stepper_set_frequency(&autochange[4].stepper);
		
		gpio_bit_reset(STEPPER5_PUL_PORT, STEPPER5_PUL_PIN);
	}
}

void STEPPER6_TIMER_HANDLER(void)
{
	if (timer_interrupt_flag_get(STEPPER6_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		timer_interrupt_flag_clear(STEPPER6_TIMER, TIMER_INT_FLAG_UP);
		
		gpio_bit_set(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
		
		stepper_set_frequency(&autochange[5].stepper);
		
		gpio_bit_reset(STEPPER6_PUL_PORT, STEPPER6_PUL_PIN);
	}
}

static void stepper_set(stepper_t *stepper, uint32_t disc)
{
	stepper->target_step = disc;
	stepper->step_count = 0;
	stepper->status = STEPPER_INCREASE;
	gpio_bit_reset(stepper->stepper_hw_info.dir_gpio, stepper->stepper_hw_info.dir_pin);//方向反
	timer_enable(stepper->stepper_hw_info.timer_periph);
}

void stepper_work(autochange_t *autochange, uint32_t disc)
{
#if 0
	if(!ptr->zero_flag)//步进电机没有在0位，不工作，直接返回
	{
		xSemaphoreGive(door->stepperMutex);//释放互斥锁
		return;
	}
#endif
	stepper_t *stepper = &autochange->stepper;
	
	if(disc >= FULL_ACC_MAX)
		stepper->acc_step_max = STEPPER_ACC_MAX;
	else
		stepper->acc_step_max = (disc / 3);
	
	if(disc == (uint32_t)AUTO_FIND)//自动找桶位
	{
		if(!(autochange->full_state & (1 << 2)))
			disc = (uint32_t)(autochange->f_dustbin_postion[DUSTBIN_3] / EACH_LAP * (float)STEPPER_SUBDIVISION);
		else if(!(autochange->full_state & (1 << 1)))
			disc = (uint32_t)(autochange->f_dustbin_postion[DUSTBIN_2] / EACH_LAP * (float)STEPPER_SUBDIVISION);
		else if(!(autochange->full_state & (1 << 0)))
			disc = (uint32_t)(autochange->f_dustbin_postion[DUSTBIN_1] / EACH_LAP * (float)STEPPER_SUBDIVISION);
		else
			return;
	}
	
	stepper_set(stepper, disc);
}
#endif
