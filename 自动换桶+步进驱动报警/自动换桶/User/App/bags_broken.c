#include "bags_broken.h"

static volatile uint8_t g_bags_broken[3] = {0xff, 0xff, 0xff}; // 0xff表示破袋没有被注册, 被注册后赋值为通道号

int bags_broken_init(bagsbroken_t *bb);

uint8_t bags_broken_get_numbers(void)
{
	uint8_t ret = 0;
	
	if(g_bags_broken[0] == 0xff)
		ret += 1;
	if(g_bags_broken[1] == 0xff)
		ret += 1;
	if(g_bags_broken[2] == 0xff)
		ret += 1;
	
	return ret;
}

int bags_broken_register(bagsbroken_t *bb, enum channel_enum_t no)
{
	uint8_t index = bags_broken_get_numbers();
	
	if(index == 0)
		return -1; 		// 没有可用破袋通道，无法注册
	
	if(g_bags_broken[0] == 0xff)
	{
		g_bags_broken[0] = no;
		bb->solenoid_valve_horizontal_move.hw_info.gpio 	= SOLENOID_VALVE_CTL_4_PORT_1;
		bb->solenoid_valve_horizontal_move.hw_info.pin 		= SOLENOID_VALVE_CTL_4_PIN_1;
		bb->solenoid_valve_horizontal_move.hw_info.gpio_rcu = SOLENOID_VALVE_CTL_4_PORT_RCU_1;

		bb->solenoid_valve_needling.hw_info.gpio 			= SOLENOID_VALVE_CTL_2_PORT_1;
		bb->solenoid_valve_needling.hw_info.pin 			= SOLENOID_VALVE_CTL_2_PIN_1;
		bb->solenoid_valve_needling.hw_info.gpio_rcu 		= SOLENOID_VALVE_CTL_2_PORT_RCU_1;

		bb->solenoid_valve_platform.hw_info.gpio 			= SOLENOID_VALVE_CTL_1_PORT_1;
		bb->solenoid_valve_platform.hw_info.pin 			= SOLENOID_VALVE_CTL_1_PIN_1;
		bb->solenoid_valve_platform.hw_info.gpio_rcu 		= SOLENOID_VALVE_CTL_1_PORT_RCU_1;

		bb->solenoid_valve_vertical_move.hw_info.gpio 		= SOLENOID_VALVE_CTL_3_PORT_1;
		bb->solenoid_valve_vertical_move.hw_info.pin 		= SOLENOID_VALVE_CTL_3_PIN_1;
		bb->solenoid_valve_vertical_move.hw_info.gpio_rcu 	= SOLENOID_VALVE_CTL_3_PORT_RCU_1;
	}
	else if(g_bags_broken[1] == 0xff)
	{
		g_bags_broken[1] = no;
		bb->solenoid_valve_horizontal_move.hw_info.gpio 	= SOLENOID_VALVE_CTL_4_PORT_2;
		bb->solenoid_valve_horizontal_move.hw_info.pin 		= SOLENOID_VALVE_CTL_4_PIN_2;
		bb->solenoid_valve_horizontal_move.hw_info.gpio_rcu = SOLENOID_VALVE_CTL_4_PORT_RCU_2;

		bb->solenoid_valve_needling.hw_info.gpio 			= SOLENOID_VALVE_CTL_2_PORT_2;
		bb->solenoid_valve_needling.hw_info.pin 			= SOLENOID_VALVE_CTL_2_PIN_2;
		bb->solenoid_valve_needling.hw_info.gpio_rcu 		= SOLENOID_VALVE_CTL_2_PORT_RCU_2;

		bb->solenoid_valve_platform.hw_info.gpio 			= SOLENOID_VALVE_CTL_1_PORT_2;
		bb->solenoid_valve_platform.hw_info.pin 			= SOLENOID_VALVE_CTL_1_PIN_2;
		bb->solenoid_valve_platform.hw_info.gpio_rcu 		= SOLENOID_VALVE_CTL_1_PORT_RCU_2;

		bb->solenoid_valve_vertical_move.hw_info.gpio 		= SOLENOID_VALVE_CTL_3_PORT_2;
		bb->solenoid_valve_vertical_move.hw_info.pin 		= SOLENOID_VALVE_CTL_3_PIN_2;
		bb->solenoid_valve_vertical_move.hw_info.gpio_rcu 	= SOLENOID_VALVE_CTL_3_PORT_RCU_2;			
	}
	else if(g_bags_broken[2] == 0xff)
	{
		g_bags_broken[2] = no;
		bb->solenoid_valve_horizontal_move.hw_info.gpio 	= SOLENOID_VALVE_CTL_4_PORT_3;
		bb->solenoid_valve_horizontal_move.hw_info.pin 		= SOLENOID_VALVE_CTL_4_PIN_3;
		bb->solenoid_valve_horizontal_move.hw_info.gpio_rcu = SOLENOID_VALVE_CTL_4_PORT_RCU_3;

		bb->solenoid_valve_needling.hw_info.gpio 			= SOLENOID_VALVE_CTL_2_PORT_3;
		bb->solenoid_valve_needling.hw_info.pin 			= SOLENOID_VALVE_CTL_2_PIN_3;
		bb->solenoid_valve_needling.hw_info.gpio_rcu 		= SOLENOID_VALVE_CTL_2_PORT_RCU_3;

		bb->solenoid_valve_platform.hw_info.gpio 			= SOLENOID_VALVE_CTL_1_PORT_3;
		bb->solenoid_valve_platform.hw_info.pin 			= SOLENOID_VALVE_CTL_1_PIN_3;
		bb->solenoid_valve_platform.hw_info.gpio_rcu 		= SOLENOID_VALVE_CTL_1_PORT_RCU_3;

		bb->solenoid_valve_vertical_move.hw_info.gpio 		= SOLENOID_VALVE_CTL_3_PORT_3;
		bb->solenoid_valve_vertical_move.hw_info.pin 		= SOLENOID_VALVE_CTL_3_PIN_3;
		bb->solenoid_valve_vertical_move.hw_info.gpio_rcu 	= SOLENOID_VALVE_CTL_3_PORT_RCU_3;	
	}
	
	bb->shake_times 									= 3;					// 抖动次数
	bb->vertical_move_up_div 							= 2;					// 垂直运动延时分频系数
	bb->shake_up_delay_time								= 220;					// 抖动上升延时
	bb->shake_down_delay_time							= 150;					// 抖动下降延时
	bb->needling_delay_time								= 2500;					// 针刺延时
	bb->horizontal_move_delay_time						= 4000;//4000;		  			// 水平移动延时
	bb->vertical_move_delay_time						= 2500;  				// 垂直移动延时
	
	bb->state 											= BAGS_BROKEN_IDLE;
	
	bags_broken_init(bb);
	
	return 0;
}

int bags_broken_init(bagsbroken_t *bb)
{
	if(!bb)
		return -1;

	solenoid_valve_init(&bb->solenoid_valve_horizontal_move);
	solenoid_valve_init(&bb->solenoid_valve_needling);
	solenoid_valve_init(&bb->solenoid_valve_platform);
	solenoid_valve_init(&bb->solenoid_valve_vertical_move);
	
	return 0;
}

void bags_broken_ready(bagsbroken_t *bb)
{
	if(solenoid_valve_state(&bb->solenoid_valve_platform) == SV_OFF)
		solenoid_valve_ctl(&bb->solenoid_valve_platform, SV_ON);
	
	bb->state = BAGS_BROKEN_READY;
}
	
void bags_broken_work(bagsbroken_t *bb)
{
	if(solenoid_valve_state(&bb->solenoid_valve_platform) == SV_OFF)
		return;
	
	bb->state = BAGS_BROKEN_BUSY;
	
	solenoid_valve_ctl(&bb->solenoid_valve_horizontal_move, SV_ON);
	bags_broken_delay(bb->horizontal_move_delay_time);
	
	solenoid_valve_ctl(&bb->solenoid_valve_vertical_move, SV_ON);
	bags_broken_delay(bb->vertical_move_delay_time);
	
	solenoid_valve_ctl(&bb->solenoid_valve_needling, SV_ON);
	bags_broken_delay(bb->needling_delay_time);
	
	solenoid_valve_ctl(&bb->solenoid_valve_platform, SV_OFF);
	solenoid_valve_ctl(&bb->solenoid_valve_vertical_move, SV_OFF);
	bags_broken_delay( (1.0f - (1.0f / (float)bb->vertical_move_up_div)) * bb->vertical_move_delay_time );
	
	for(int i = 0; i < bb->shake_times; i += 1)
	{
		solenoid_valve_ctl(&bb->solenoid_valve_vertical_move, SV_ON);
		bags_broken_delay(bb->shake_down_delay_time);
		solenoid_valve_ctl(&bb->solenoid_valve_vertical_move, SV_OFF);
		bags_broken_delay(bb->shake_up_delay_time);
	}
	int32_t tmp = (int32_t)bb->vertical_move_delay_time - 
				  (int32_t)(bb->vertical_move_delay_time / bb->vertical_move_up_div) -
				  (int32_t)(bb->shake_down_delay_time + bb->shake_up_delay_time) * (int32_t)bb->shake_times;
	if(tmp <= 0)
		tmp = 1000;
	tmp += 1000;
	
	bags_broken_delay((uint32_t)tmp);
	
	bb->state = BAGS_BROKEN_IDLE;
#if 0	
	bags_broken_delay(3000);
	
	solenoid_valve_ctl(&bb->solenoid_valve_horizontal_move, SV_OFF);
	bags_broken_delay(bb->horizontal_move_delay_time);
	
	solenoid_valve_ctl(&bb->solenoid_valve_needling, SV_OFF);
	bags_broken_delay(bb->needling_delay_time);
#endif	
	// bb->state = BAGS_BROKEN_IDLE;
}

void bags_broken_over(bagsbroken_t *bb)
{
	//bags_broken_delay(5000);
	
	solenoid_valve_ctl(&bb->solenoid_valve_horizontal_move, SV_OFF);
	bags_broken_delay(bb->horizontal_move_delay_time);
	
	solenoid_valve_ctl(&bb->solenoid_valve_needling, SV_OFF);
	//bags_broken_delay(bb->needling_delay_time);
}

void bags_broken_reset(bagsbroken_t *bb)  // 可以完善
{
#if 0
	uint8_t flag[4] = {0};
	
	if(solenoid_valve_state(&bb->solenoid_valve_platform) == SV_ON)
		flag[3] = 1;
	if(solenoid_valve_state(&bb->solenoid_valve_horizontal_move) == SV_ON)
		flag[2] = 1;
	if(solenoid_valve_state(&bb->solenoid_valve_vertical_move) == SV_ON)
		flag[1] = 1;
	if(solenoid_valve_state(&bb->solenoid_valve_needling) == SV_ON)
		flag[0] = 1;
	
	if(flag[0] == 0 && flag[1] == 0 && flag[2] == 0 && flag[3] == 0) 		// 第一种情况 本来就没工作
		return;
	else if(flag[0] == 0 && flag[1] == 0 && flag[2] == 0 && flag[3] == 1) 	// 第二种情况 只有托盘放下来了
		solenoid_valve_ctl(&bb->solenoid_valve_platform, SV_OFF);			// 让托盘回去
	else if(flag[0] == 0 && flag[1] == 0 && flag[2] == 1 && flag[3] == 0)   // 第三种情况 
#endif	
	if(solenoid_valve_state(&bb->solenoid_valve_platform) == SV_ON)
		solenoid_valve_ctl(&bb->solenoid_valve_platform, SV_OFF);
	
	bb->state = BAGS_BROKEN_IDLE;
}

enum bags_broken_state_enum_t bags_broken_get_state(bagsbroken_t *bb)
{
	return bb->state;
}
