#include "CD4051.h"
#include "74hc595.h"
#include "delay.h"

void cd4051_init(void)
{
	rcu_periph_clock_enable(CD4051_CH1_INPUT_GPIO_RCU);
	rcu_periph_clock_enable(CD4051_CH2_INPUT_GPIO_RCU);
	rcu_periph_clock_enable(CD4051_CH3_INPUT_GPIO_RCU);
	rcu_periph_clock_enable(CD4051_CH4_INPUT_GPIO_RCU);
	rcu_periph_clock_enable(CD4051_CH5_INPUT_GPIO_RCU);
	rcu_periph_clock_enable(CD4051_CH6_INPUT_GPIO_RCU);
	//
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(CD4051_ALM_DETECT_RCU);
	
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	gpio_init(CD4051_CH1_INPUT_GPIO_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_CH1_INPUT_PIN);
	gpio_init(CD4051_CH2_INPUT_GPIO_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_CH2_INPUT_PIN);
	gpio_init(CD4051_CH3_INPUT_GPIO_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_CH3_INPUT_PIN);
	gpio_init(CD4051_CH4_INPUT_GPIO_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_CH4_INPUT_PIN);
	gpio_init(CD4051_CH5_INPUT_GPIO_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_CH5_INPUT_PIN);
	gpio_init(CD4051_CH6_INPUT_GPIO_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_CH6_INPUT_PIN);
	gpio_init(CD4051_ALM_DETECT_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, CD4051_ALM_DETECT_PIN);
}

FlagStatus cd4051_detect(enum channel_enum_t ch, cd4051_detect_enum_t fun)
{
	uint8_t tmp_fun = fun;
	uint32_t gpio, pin;
	
	switch(ch)
	{
	case CHANNEL_1 :
		if(fun == STEPPER_ALM_DET){
			ch = STEPPER_ALARM;
			tmp_fun = 7;
			gpio = CD4051_ALM_DETECT_GPIO;
			pin  = CD4051_ALM_DETECT_PIN;
			break;
		}
		gpio = CD4051_CH1_INPUT_GPIO;
		pin  = CD4051_CH1_INPUT_PIN;
		break;
	case CHANNEL_2 :
		if(fun == STEPPER_ALM_DET){
			ch = STEPPER_ALARM;
			tmp_fun = 5;
			gpio = CD4051_ALM_DETECT_GPIO;
			pin  = CD4051_ALM_DETECT_PIN;
			break;
		}
		gpio = CD4051_CH2_INPUT_GPIO;
		pin  = CD4051_CH2_INPUT_PIN;
		break;
	case CHANNEL_3 :
		if(fun == STEPPER_ALM_DET){
			tmp_fun = 3;
			gpio = CD4051_ALM_DETECT_GPIO;
			pin  = CD4051_ALM_DETECT_PIN;
			break;
		}
		gpio = CD4051_CH3_INPUT_GPIO;
		pin  = CD4051_CH3_INPUT_PIN;
		break;
	case CHANNEL_4 :
		if(fun == STEPPER_ALM_DET){
			ch = STEPPER_ALARM;
			tmp_fun = 0;
			gpio = CD4051_ALM_DETECT_GPIO;
			pin  = CD4051_ALM_DETECT_PIN;
			break;
		}
		gpio = CD4051_CH4_INPUT_GPIO;
		pin  = CD4051_CH4_INPUT_PIN;
		break;
	case CHANNEL_5 :
		
		if(fun == STEPPER_ALM_DET){
			ch = STEPPER_ALARM;
			tmp_fun = 1;
			gpio = CD4051_ALM_DETECT_GPIO;
			pin  = CD4051_ALM_DETECT_PIN;
			break;
		}
		gpio = CD4051_CH5_INPUT_GPIO;
		pin  = CD4051_CH5_INPUT_PIN;
		break;
	case CHANNEL_6 :
		
		if(fun == STEPPER_ALM_DET){
			ch = STEPPER_ALARM;
			tmp_fun = 2;
			gpio = CD4051_ALM_DETECT_GPIO;
			pin  = CD4051_ALM_DETECT_PIN;
			break;
		}
		gpio = CD4051_CH6_INPUT_GPIO;
		pin  = CD4051_CH6_INPUT_PIN;
		break;
	default :
		break;
	}
	
	
	_74hc595_ctl(ch, CD4051_A0, (SWITCH_STATE)(tmp_fun & 1), 0);
	_74hc595_ctl(ch, CD4051_A1, (SWITCH_STATE)((tmp_fun & 2) >> 1), 0);
	_74hc595_ctl(ch, CD4051_A2, (SWITCH_STATE)((tmp_fun & 4) >> 2), 1);
	
	delay_nus(100);
	
	FlagStatus ret = gpio_input_bit_get(gpio, pin);
	if((fun == KEY_DET) || (fun == DOOR_EXIST_DET) || (fun == STEPPER_ZERO_DET) || (fun == STEPPER_TAIL_DET) ||
	   (fun == DOOR_OPEN_DET) || (fun == DOOR_CLOSE_DET) || (fun == IR_DET)
	)
	{
		ret ^= SET;
	}
	
	return ret;
}
