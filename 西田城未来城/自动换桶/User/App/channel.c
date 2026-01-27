#include "channel.h"
#include "app.h"

channel_t channel[6] = {
	[0] = {
		/* 自动换桶翻斗推杆 */
        .autochange.linear_actuator.hw_info.ctl1_gpio 		= INTERFACE1_TIPPING_MOTOR_CTL_PORT1,
		.autochange.linear_actuator.hw_info.ctl1_pin 		= INTERFACE1_TIPPING_MOTOR_CTL_PIN1,
		.autochange.linear_actuator.hw_info.ctl1_gpio_rcu 	= INTERFACE1_TIPPING_MOTOR_CTL1_PORT_RCU,
		.autochange.linear_actuator.hw_info.ctl2_gpio 		= INTERFACE1_TIPPING_MOTOR_CTL_PORT2,
		.autochange.linear_actuator.hw_info.ctl2_pin 		= INTERFACE1_TIPPING_MOTOR_CTL_PIN2,
		.autochange.linear_actuator.hw_info.ctl2_gpio_rcu 	= INTERFACE1_TIPPING_MOTOR_CTL2_PORT_RCU,
		/* 自动换桶步进电机 */
		.autochange.stepper.status							= STEPPER_IDLE,
		.autochange.stepper.step_count 						= 0,
		.autochange.stepper.acc_step_max 					= STEPPER_ACC_MAX,
		.autochange.stepper.target_step 					= 0,
		.autochange.stepper.stepper_hw_info.timer_periph 	= STEPPER1_TIMER,
		.autochange.stepper.stepper_hw_info.timer_rcu 		= STEPPER1_TIMER_RCU,
		.autochange.stepper.stepper_hw_info.dir_gpio 		= STEPPER1_DIR_PORT,
		.autochange.stepper.stepper_hw_info.dir_pin 		= STEPPER1_DIR_PIN,
		.autochange.stepper.stepper_hw_info.pul_gpio 		= STEPPER1_PUL_PORT,
		.autochange.stepper.stepper_hw_info.pul_pin 		= STEPPER1_PUL_PIN,
		.autochange.stepper.stepper_hw_info.dir_gpio_rcu 	= STEPPER1_DIR_PORT_RCU,
		.autochange.stepper.stepper_hw_info.pul_gpio_rcu 	= STEPPER1_PUL_PORT_RCU,
		.autochange.stepper.stepper_hw_info.timer_irq 		= STEPPER1_TIMER_IRQ,
		/* 门板投口杆 */
		.door.linear_actuator.hw_info.ctl1_gpio 			= INTERFACE1_DOOR_MOTOR_CTL_PORT1,
		.door.linear_actuator.hw_info.ctl1_pin 				= INTERFACE1_DOOR_MOTOR_CTL_PIN1,
		.door.linear_actuator.hw_info.ctl1_gpio_rcu 		= INTERFACE1_DOOR_MOTOR_CTL1_PORT_RCU,
		.door.linear_actuator.hw_info.ctl2_gpio 			= INTERFACE1_DOOR_MOTOR_CTL_PORT2,
		.door.linear_actuator.hw_info.ctl2_pin 				= INTERFACE1_DOOR_MOTOR_CTL_PIN2,
		.door.linear_actuator.hw_info.ctl2_gpio_rcu 		= INTERFACE1_DOOR_MOTOR_CTL2_PORT_RCU,
		/* 满筒灯 */
		.door.full_light.hw_info.gpio 						= WS2812B_1_DATA_PORT,
		.door.full_light.hw_info.pin 						= WS2812B_1_DATA_PIN,
		.door.full_light.hw_info.gpio_rcu 					= WS2812B_1_DATA_PORT_RCU,
		.door.full_light.numbers 							= 16,
		
		.bags_broken 										= (void*)-1,//NULL,//mmmm
		.no 												= CHANNEL_1,
		
    },
	[1] = {
		/* 自动换桶翻斗推杆 */
        .autochange.linear_actuator.hw_info.ctl1_gpio 		= INTERFACE2_TIPPING_MOTOR_CTL_PORT1,
		.autochange.linear_actuator.hw_info.ctl1_pin 		= INTERFACE2_TIPPING_MOTOR_CTL_PIN1,
		.autochange.linear_actuator.hw_info.ctl1_gpio_rcu 	= INTERFACE2_TIPPING_MOTOR_CTL1_PORT_RCU,
		.autochange.linear_actuator.hw_info.ctl2_gpio 		= INTERFACE2_TIPPING_MOTOR_CTL_PORT2,
		.autochange.linear_actuator.hw_info.ctl2_pin 		= INTERFACE2_TIPPING_MOTOR_CTL_PIN2,
		.autochange.linear_actuator.hw_info.ctl2_gpio_rcu 	= INTERFACE2_TIPPING_MOTOR_CTL2_PORT_RCU,
		/* 自动换桶步进电机 */
		.autochange.stepper.status							= STEPPER_IDLE,
		.autochange.stepper.step_count 						= 0,
		.autochange.stepper.acc_step_max 					= STEPPER_ACC_MAX,
		.autochange.stepper.target_step 					= 0,
		.autochange.stepper.stepper_hw_info.timer_periph 	= STEPPER2_TIMER,
		.autochange.stepper.stepper_hw_info.timer_rcu 		= STEPPER2_TIMER_RCU,
		.autochange.stepper.stepper_hw_info.dir_gpio 		= STEPPER2_DIR_PORT,
		.autochange.stepper.stepper_hw_info.dir_pin 		= STEPPER2_DIR_PIN,
		.autochange.stepper.stepper_hw_info.pul_gpio 		= STEPPER2_PUL_PORT,
		.autochange.stepper.stepper_hw_info.pul_pin 		= STEPPER2_PUL_PIN,
		.autochange.stepper.stepper_hw_info.dir_gpio_rcu 	= STEPPER2_DIR_PORT_RCU,
		.autochange.stepper.stepper_hw_info.pul_gpio_rcu 	= STEPPER2_PUL_PORT_RCU,
		.autochange.stepper.stepper_hw_info.timer_irq 		= STEPPER2_TIMER_IRQ,
		/* 门板投口杆 */
		.door.linear_actuator.hw_info.ctl1_gpio 			= INTERFACE2_DOOR_MOTOR_CTL_PORT1,
		.door.linear_actuator.hw_info.ctl1_pin 				= INTERFACE2_DOOR_MOTOR_CTL_PIN1,
		.door.linear_actuator.hw_info.ctl1_gpio_rcu 		= INTERFACE2_DOOR_MOTOR_CTL1_PORT_RCU,
		.door.linear_actuator.hw_info.ctl2_gpio 			= INTERFACE2_DOOR_MOTOR_CTL_PORT2,
		.door.linear_actuator.hw_info.ctl2_pin 				= INTERFACE2_DOOR_MOTOR_CTL_PIN2,
		.door.linear_actuator.hw_info.ctl2_gpio_rcu 		= INTERFACE2_DOOR_MOTOR_CTL2_PORT_RCU,
		/* 满筒灯 */
		.door.full_light.hw_info.gpio 						= WS2812B_2_DATA_PORT,
		.door.full_light.hw_info.pin 						= WS2812B_2_DATA_PIN,
		.door.full_light.hw_info.gpio_rcu 					= WS2812B_2_DATA_PORT_RCU,
		.door.full_light.numbers 							= 16,
		
		.bags_broken 										= NULL,
		.no 												= CHANNEL_2,
    },
	[2] = {
		/* 自动换桶翻斗推杆 */
        .autochange.linear_actuator.hw_info.ctl1_gpio 		= INTERFACE3_TIPPING_MOTOR_CTL_PORT1,
		.autochange.linear_actuator.hw_info.ctl1_pin 		= INTERFACE3_TIPPING_MOTOR_CTL_PIN1,
		.autochange.linear_actuator.hw_info.ctl1_gpio_rcu 	= INTERFACE3_TIPPING_MOTOR_CTL1_PORT_RCU,
		.autochange.linear_actuator.hw_info.ctl2_gpio 		= INTERFACE3_TIPPING_MOTOR_CTL_PORT2,
		.autochange.linear_actuator.hw_info.ctl2_pin 		= INTERFACE3_TIPPING_MOTOR_CTL_PIN2,
		.autochange.linear_actuator.hw_info.ctl2_gpio_rcu 	= INTERFACE3_TIPPING_MOTOR_CTL2_PORT_RCU,
		/* 自动换桶步进电机 */
		.autochange.stepper.status							= STEPPER_IDLE,
		.autochange.stepper.step_count 						= 0,
		.autochange.stepper.acc_step_max 					= STEPPER_ACC_MAX,
		.autochange.stepper.target_step 					= 0,
		.autochange.stepper.stepper_hw_info.timer_periph 	= STEPPER3_TIMER,
		.autochange.stepper.stepper_hw_info.timer_rcu 		= STEPPER3_TIMER_RCU,
		.autochange.stepper.stepper_hw_info.dir_gpio 		= STEPPER3_DIR_PORT,
		.autochange.stepper.stepper_hw_info.dir_pin 		= STEPPER3_DIR_PIN,
		.autochange.stepper.stepper_hw_info.pul_gpio 		= STEPPER3_PUL_PORT,
		.autochange.stepper.stepper_hw_info.pul_pin 		= STEPPER3_PUL_PIN,
		.autochange.stepper.stepper_hw_info.dir_gpio_rcu 	= STEPPER3_DIR_PORT_RCU,
		.autochange.stepper.stepper_hw_info.pul_gpio_rcu 	= STEPPER3_PUL_PORT_RCU,
		.autochange.stepper.stepper_hw_info.timer_irq 		= STEPPER3_TIMER_IRQ,
		/* 门板投口杆 */
		.door.linear_actuator.hw_info.ctl1_gpio 			= INTERFACE3_DOOR_MOTOR_CTL_PORT1,
		.door.linear_actuator.hw_info.ctl1_pin 				= INTERFACE3_DOOR_MOTOR_CTL_PIN1,
		.door.linear_actuator.hw_info.ctl1_gpio_rcu 		= INTERFACE3_DOOR_MOTOR_CTL1_PORT_RCU,
		.door.linear_actuator.hw_info.ctl2_gpio 			= INTERFACE3_DOOR_MOTOR_CTL_PORT2,
		.door.linear_actuator.hw_info.ctl2_pin 				= INTERFACE3_DOOR_MOTOR_CTL_PIN2,
		.door.linear_actuator.hw_info.ctl2_gpio_rcu 		= INTERFACE3_DOOR_MOTOR_CTL2_PORT_RCU,
		/* 满筒灯 */
		.door.full_light.hw_info.gpio 						= WS2812B_3_DATA_PORT,
		.door.full_light.hw_info.pin 						= WS2812B_3_DATA_PIN,
		.door.full_light.hw_info.gpio_rcu 					= WS2812B_3_DATA_PORT_RCU,
		.door.full_light.numbers 							= 16,
		
		.bags_broken 										= NULL,
		.no 												= CHANNEL_3,
    },
	[3] = {
		/* 自动换桶翻斗推杆 */
        .autochange.linear_actuator.hw_info.ctl1_gpio 		= INTERFACE4_TIPPING_MOTOR_CTL_PORT1,
		.autochange.linear_actuator.hw_info.ctl1_pin 		= INTERFACE4_TIPPING_MOTOR_CTL_PIN1,
		.autochange.linear_actuator.hw_info.ctl1_gpio_rcu 	= INTERFACE4_TIPPING_MOTOR_CTL1_PORT_RCU,
		.autochange.linear_actuator.hw_info.ctl2_gpio 		= INTERFACE4_TIPPING_MOTOR_CTL_PORT2,
		.autochange.linear_actuator.hw_info.ctl2_pin 		= INTERFACE4_TIPPING_MOTOR_CTL_PIN2,
		.autochange.linear_actuator.hw_info.ctl2_gpio_rcu 	= INTERFACE4_TIPPING_MOTOR_CTL2_PORT_RCU,
		/* 自动换桶步进电机 */
		.autochange.stepper.status							= STEPPER_IDLE,
		.autochange.stepper.step_count 						= 0,
		.autochange.stepper.acc_step_max 					= STEPPER_ACC_MAX,
		.autochange.stepper.target_step 					= 0,
		.autochange.stepper.stepper_hw_info.timer_periph 	= STEPPER4_TIMER,
		.autochange.stepper.stepper_hw_info.timer_rcu 		= STEPPER4_TIMER_RCU,
		.autochange.stepper.stepper_hw_info.dir_gpio 		= STEPPER4_DIR_PORT,
		.autochange.stepper.stepper_hw_info.dir_pin 		= STEPPER4_DIR_PIN,
		.autochange.stepper.stepper_hw_info.pul_gpio 		= STEPPER4_PUL_PORT,
		.autochange.stepper.stepper_hw_info.pul_pin 		= STEPPER4_PUL_PIN,
		.autochange.stepper.stepper_hw_info.dir_gpio_rcu 	= STEPPER4_DIR_PORT_RCU,
		.autochange.stepper.stepper_hw_info.pul_gpio_rcu 	= STEPPER4_PUL_PORT_RCU,
		.autochange.stepper.stepper_hw_info.timer_irq 		= STEPPER4_TIMER_IRQ,
		/* 门板投口杆 */
		.door.linear_actuator.hw_info.ctl1_gpio 			= INTERFACE4_DOOR_MOTOR_CTL_PORT1,
		.door.linear_actuator.hw_info.ctl1_pin 				= INTERFACE4_DOOR_MOTOR_CTL_PIN1,
		.door.linear_actuator.hw_info.ctl1_gpio_rcu 		= INTERFACE4_DOOR_MOTOR_CTL1_PORT_RCU,
		.door.linear_actuator.hw_info.ctl2_gpio 			= INTERFACE4_DOOR_MOTOR_CTL_PORT2,
		.door.linear_actuator.hw_info.ctl2_pin 				= INTERFACE4_DOOR_MOTOR_CTL_PIN2,
		.door.linear_actuator.hw_info.ctl2_gpio_rcu 		= INTERFACE4_DOOR_MOTOR_CTL2_PORT_RCU,
		/* 满筒灯 */
		.door.full_light.hw_info.gpio 						= WS2812B_4_DATA_PORT,
		.door.full_light.hw_info.pin 						= WS2812B_4_DATA_PIN,
		.door.full_light.hw_info.gpio_rcu 					= WS2812B_4_DATA_PORT_RCU,
		.door.full_light.numbers 							= 16,
		
		.bags_broken 										= (void*)-1,
		.no 												= CHANNEL_4,
    },
	[4] = {
		/* 自动换桶翻斗推杆 */
        .autochange.linear_actuator.hw_info.ctl1_gpio 		= INTERFACE5_TIPPING_MOTOR_CTL_PORT1,
		.autochange.linear_actuator.hw_info.ctl1_pin 		= INTERFACE5_TIPPING_MOTOR_CTL_PIN1,
		.autochange.linear_actuator.hw_info.ctl1_gpio_rcu 	= INTERFACE5_TIPPING_MOTOR_CTL1_PORT_RCU,
		.autochange.linear_actuator.hw_info.ctl2_gpio 		= INTERFACE5_TIPPING_MOTOR_CTL_PORT2,
		.autochange.linear_actuator.hw_info.ctl2_pin 		= INTERFACE5_TIPPING_MOTOR_CTL_PIN2,
		.autochange.linear_actuator.hw_info.ctl2_gpio_rcu 	= INTERFACE5_TIPPING_MOTOR_CTL2_PORT_RCU,
		/* 自动换桶步进电机 */
		.autochange.stepper.status							= STEPPER_IDLE,
		.autochange.stepper.step_count 						= 0,
		.autochange.stepper.acc_step_max 					= STEPPER_ACC_MAX,
		.autochange.stepper.target_step 					= 0,
		.autochange.stepper.stepper_hw_info.timer_periph 	= STEPPER5_TIMER,
		.autochange.stepper.stepper_hw_info.timer_rcu 		= STEPPER5_TIMER_RCU,
		.autochange.stepper.stepper_hw_info.dir_gpio 		= STEPPER5_DIR_PORT,
		.autochange.stepper.stepper_hw_info.dir_pin 		= STEPPER5_DIR_PIN,
		.autochange.stepper.stepper_hw_info.pul_gpio 		= STEPPER5_PUL_PORT,
		.autochange.stepper.stepper_hw_info.pul_pin 		= STEPPER5_PUL_PIN,
		.autochange.stepper.stepper_hw_info.dir_gpio_rcu 	= STEPPER5_DIR_PORT_RCU,
		.autochange.stepper.stepper_hw_info.pul_gpio_rcu 	= STEPPER5_PUL_PORT_RCU,
		.autochange.stepper.stepper_hw_info.timer_irq 		= STEPPER5_TIMER_IRQ,
		/* 门板投口杆 */
		.door.linear_actuator.hw_info.ctl1_gpio 			= INTERFACE5_DOOR_MOTOR_CTL_PORT1,
		.door.linear_actuator.hw_info.ctl1_pin 				= INTERFACE5_DOOR_MOTOR_CTL_PIN1,
		.door.linear_actuator.hw_info.ctl1_gpio_rcu 		= INTERFACE5_DOOR_MOTOR_CTL1_PORT_RCU,
		.door.linear_actuator.hw_info.ctl2_gpio 			= INTERFACE5_DOOR_MOTOR_CTL_PORT2,
		.door.linear_actuator.hw_info.ctl2_pin 				= INTERFACE5_DOOR_MOTOR_CTL_PIN2,
		.door.linear_actuator.hw_info.ctl2_gpio_rcu 		= INTERFACE5_DOOR_MOTOR_CTL2_PORT_RCU,
		/* 满筒灯 */
		.door.full_light.hw_info.gpio 						= WS2812B_5_DATA_PORT,
		.door.full_light.hw_info.pin 						= WS2812B_5_DATA_PIN,
		.door.full_light.hw_info.gpio_rcu 					= WS2812B_5_DATA_PORT_RCU,
		.door.full_light.numbers 							= 16,
		
		.bags_broken 										= NULL,
		.no 												= CHANNEL_5,
    },
	[5] = {
		/* 自动换桶翻斗推杆 */
        .autochange.linear_actuator.hw_info.ctl1_gpio 		= INTERFACE6_TIPPING_MOTOR_CTL_PORT1,
		.autochange.linear_actuator.hw_info.ctl1_pin 		= INTERFACE6_TIPPING_MOTOR_CTL_PIN1,
		.autochange.linear_actuator.hw_info.ctl1_gpio_rcu 	= INTERFACE6_TIPPING_MOTOR_CTL1_PORT_RCU,
		.autochange.linear_actuator.hw_info.ctl2_gpio 		= INTERFACE6_TIPPING_MOTOR_CTL_PORT2,
		.autochange.linear_actuator.hw_info.ctl2_pin 		= INTERFACE6_TIPPING_MOTOR_CTL_PIN2,
		.autochange.linear_actuator.hw_info.ctl2_gpio_rcu 	= INTERFACE6_TIPPING_MOTOR_CTL2_PORT_RCU,
		/* 自动换桶步进电机 */
		.autochange.stepper.status							= STEPPER_IDLE,
		.autochange.stepper.step_count 						= 0,
		.autochange.stepper.acc_step_max 					= STEPPER_ACC_MAX,
		.autochange.stepper.target_step 					= 0,
		.autochange.stepper.stepper_hw_info.timer_periph 	= STEPPER6_TIMER,
		.autochange.stepper.stepper_hw_info.timer_rcu 		= STEPPER6_TIMER_RCU,
		.autochange.stepper.stepper_hw_info.dir_gpio 		= STEPPER6_DIR_PORT,
		.autochange.stepper.stepper_hw_info.dir_pin 		= STEPPER6_DIR_PIN,
		.autochange.stepper.stepper_hw_info.pul_gpio 		= STEPPER6_PUL_PORT,
		.autochange.stepper.stepper_hw_info.pul_pin 		= STEPPER6_PUL_PIN,
		.autochange.stepper.stepper_hw_info.dir_gpio_rcu 	= STEPPER6_DIR_PORT_RCU,
		.autochange.stepper.stepper_hw_info.pul_gpio_rcu 	= STEPPER6_PUL_PORT_RCU,
		.autochange.stepper.stepper_hw_info.timer_irq 		= STEPPER6_TIMER_IRQ,
		/* 门板投口杆 */
		.door.linear_actuator.hw_info.ctl1_gpio 			= INTERFACE6_DOOR_MOTOR_CTL_PORT1,
		.door.linear_actuator.hw_info.ctl1_pin 				= INTERFACE6_DOOR_MOTOR_CTL_PIN1,
		.door.linear_actuator.hw_info.ctl1_gpio_rcu 		= INTERFACE6_DOOR_MOTOR_CTL1_PORT_RCU,
		.door.linear_actuator.hw_info.ctl2_gpio 			= INTERFACE6_DOOR_MOTOR_CTL_PORT2,
		.door.linear_actuator.hw_info.ctl2_pin 				= INTERFACE6_DOOR_MOTOR_CTL_PIN2,
		.door.linear_actuator.hw_info.ctl2_gpio_rcu 		= INTERFACE6_DOOR_MOTOR_CTL2_PORT_RCU,
		/* 满筒灯 */
		.door.full_light.hw_info.gpio 						= WS2812B_6_DATA_PORT,
		.door.full_light.hw_info.pin 						= WS2812B_6_DATA_PIN,
		.door.full_light.hw_info.gpio_rcu 					= WS2812B_6_DATA_PORT_RCU,
		.door.full_light.numbers 							= 16,
		
		.bags_broken 										= (void*)-1,//设置破袋
		.no 												= CHANNEL_6,
    },
};

#if 0
static void task_channel(void *arg)
{
	channel_t *channel = (channel_t *)arg;
	FlagStatus ret;
	
	vTaskDelay(3000);
	while(1)
	{
		taskENTER_CRITICAL();
		ret = cd4051_detect(channel->no, IR_DET);
		taskEXIT_CRITICAL();
		
		if(ret == SET)
			linear_actuator_ctl(&channel->door.linear_actuator, LA_OPEN);
		//else
			
		
		vTaskDelay(100);
	}
}
#else


void task_channel(void *arg)
{
	BaseType_t 	ret = pdTRUE;
	uint32_t 	read_event = 0;
	//uint8_t 	bb_flag_tmp = 0;
	
	channel_t *channel = (channel_t *)arg;
	
	stepper_find_zero(channel);
	//WAIT_FOR_STEPPER(channel);
	vTaskDelay(800);
	channel_full_detect(channel);
	taskENTER_CRITICAL();
	full_light_show(channel);
	taskEXIT_CRITICAL();
	
	while(1)
	{
#if 0
		if(bb_flag_tmp)
		{
			bags_broken_over(channel->bags_broken);
			bb_flag_tmp = 0;
		}
#endif		
		taskENTER_CRITICAL();
		_74hc595_ctl(channel->no, DOOR_PCB_LED, OFF, 1);
		taskEXIT_CRITICAL();
		ret = xTaskNotifyWait(~DOOR_ALL_EVENTS, 0xffffffff, &read_event, portMAX_DELAY); // 用任务通知代替事件组，等待事件发生
		if(ret == pdFALSE)
			continue;
		
		taskENTER_CRITICAL();
		_74hc595_ctl(channel->no, DOOR_PCB_LED, ON, 1);
		taskEXIT_CRITICAL();
		
		switch(read_event & DOOR_ALL_EVENTS)
		{
		case DOOR_OPEN :
			taskENTER_CRITICAL();
			channel->door.ps_detect_flag = PROXIMITY_SWITCH_ON;
			taskEXIT_CRITICAL();
			linear_actuator_ctl(&channel->door.linear_actuator, LA_OPEN);
			break;
		case DOOR_CLOSE :
			taskENTER_CRITICAL();
			channel->door.ps_detect_flag = PROXIMITY_SWITCH_ON;
			taskEXIT_CRITICAL();
			linear_actuator_ctl(&channel->door.linear_actuator, LA_CLOSE);
			break;
		case DOOR_STOP :
			linear_actuator_ctl(&channel->door.linear_actuator, LA_STOP);
			break;
		case STEPPER_ZERO :
			stepper_find_zero(channel);
			break;
		case STEPPER_FULL_DETECT :
			channel_full_detect(channel);
			taskENTER_CRITICAL();
			full_light_show(channel);
			taskEXIT_CRITICAL();
			break;
		case STEPPER_THROW_1 :
			throw_garbage(channel, STEPPER_THROW_1);
			break;
		case STEPPER_THROW_2 :
			throw_garbage(channel, STEPPER_THROW_2);
			break;
		case STEPPER_THROW_3 :
			throw_garbage(channel, STEPPER_THROW_3);
			break;
		case STEPPER_THROW_4 :
			throw_garbage(channel, STEPPER_THROW_4);
			break;
		case STEPPER_THROW_AUTO :
			taskENTER_CRITICAL();
			if(channel->autochange.full_state == 0x07)
			{
				taskEXIT_CRITICAL();
				break;
			}
			taskEXIT_CRITICAL();
			throw_garbage(channel, STEPPER_THROW_AUTO);
			break;
		case _BAGS_BROKEN_READY :
			if(channel->bags_broken != NULL && channel->bags_broken != (void *)-1)
				bags_broken_ready(channel->bags_broken);
			break;
		case _BAGS_BROKEN_RESET :
			if(channel->bags_broken != NULL && channel->bags_broken != (void *)-1)
				bags_broken_reset(channel->bags_broken);
			break;
		case _BAGS_BROKEN_WORK :
			if(channel->bags_broken != NULL && channel->bags_broken != (void *)-1)
			{
				bags_broken_work(channel->bags_broken);
			}
			break;
		//case _BAGS_OVER :
			//bags_broken_over(channel->bags_broken);
			//break;
		case GS_ENABLE :
			taskENTER_CRITICAL();
			channel->door.ir_grating_enable_flag = IR_ENABLE;
			taskEXIT_CRITICAL();
			break;
		case GS_DISABLE :
			taskENTER_CRITICAL();
			channel->door.ir_grating_enable_flag = IR_DISABLE;
			taskEXIT_CRITICAL();
			break;
		default :
			break;
		}
	}
}
#endif

static void task_channel_detect(void *arg)
{
	TickType_t xLastWakeTime;
    const TickType_t delay_100ms = pdMS_TO_TICKS(100);//100
    xLastWakeTime = xTaskGetTickCount();
	
	channel_t *channel = (channel_t *)arg;
	enum ir_enable_enum_t gs_enable_flag;
	enum proximity_switch_detect_enum_t proximity_switch_flag;

	while(1)
	{
		taskENTER_CRITICAL();
		gs_enable_flag = channel->door.ir_grating_enable_flag;
		proximity_switch_flag = channel->door.ps_detect_flag;
		taskEXIT_CRITICAL();
		
		if(gs_enable_flag == IR_ENABLE)
		{
			gs_detect(channel);
		}
		if(proximity_switch_flag == PROXIMITY_SWITCH_ON)
		{
			proximity_switch_detect(channel);
		}
		
		key_detect(channel);
		
		door_key_led(channel);
		
		stepper_alm_detect(channel);
		
		vTaskDelayUntil(&xLastWakeTime, delay_100ms);
	}
}

int channel_init(enum channel_enum_t no)
{
	if(no > 6)
		return -1;
	channel_t *pch = &channel[no];
	
	autochange_init(&pch->autochange, (uint8_t)no);
	door_init(&pch->door);
	
	if(pch->bags_broken == (void *)-1)
	{
		if(bags_broken_get_numbers())
		{	
			pch->bags_broken = pvPortMalloc(sizeof(bagsbroken_t));
			if(pch->bags_broken == NULL)
			{
				return -2;
			}
			
			if(bags_broken_register(pch->bags_broken, no) != 0)
				return -3;
		}
	}
	
	linear_actuator_ctl(&pch->door.linear_actuator, LA_CLOSE);
	linear_actuator_ctl(&pch->autochange.linear_actuator, LA_CLOSE);
	
	char tmp_name[] = "task_channel1";
	tmp_name[12] += (int)no;
	xTaskCreate(task_channel, tmp_name, 128, (void *)&channel[no], 10, &channel[no].task_handle);
	char tmp_name2[] = "task_channel_detect1";
	tmp_name2[19] += (int)no;
	xTaskCreate(task_channel_detect, tmp_name2, 512, (void *)&channel[no], 10, &channel[no].detect_handle);
	
	taskENTER_CRITICAL();
	
	_74hc595_ctl(no, VCC_12V, ON, 0);
	_74hc595_ctl(no, VCC_5V, ON, 0);
	_74hc595_ctl(no, VCC_MOTOR, ON, 1);
	
	taskEXIT_CRITICAL();

	return 0;
}
#if 0
void shouzhen_task_(void *arg)
{
	BaseType_t ret;
	uint32_t read_event;
	
	while(1)
	{
		ret = xTaskNotifyWait(~DOOR_ALL_EVENTS, 0xffffffff, &read_event, portMAX_DELAY); // 用任务通知代替事件组，等待事件发生
		if(ret == pdFALSE)
			continue;
		
		if(read_event == _BAGS_OVER)
		{
			bags_broken_over(channel->bags_broken);
		}
	}
}
#endif
