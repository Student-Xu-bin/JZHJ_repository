#include "garbage_pitch.h"

garbage_pitch::garbage_pitch(int no,
							 uint8_t channel,
							 rcu_periph_enum rcu_port1, rcu_periph_enum rcu_port2, uint32_t port1, uint32_t port2, uint32_t pin1, uint32_t pin2,
							 rcu_periph_enum key_rcu_port, uint32_t key_port, uint32_t key_pin,
							 rcu_periph_enum full_light_rcu_port, uint32_t full_light_port, uint32_t full_light_pin)
{
	pitch_no = no;
	pitch_error_flag = 0;
	
	adc_channel = channel;
	adc_zero = 0;
	close_pitch_benchmark_value = 0;
	open_pitch_benchmark_value = 0;
	current_init_flag = 0;
	
	motor.rcu_port1 = rcu_port1;
	motor.rcu_port2 = rcu_port2;
	motor.port1 = port1;
	motor.port2 = port2;
	motor.pin1 = pin1;
	motor.pin2 = pin2;
	motor_status = motor_status_t::MOTOR_STOP;
	motor_init();
	
	key.rcu_port = key_rcu_port;
	key.port = key_port;
	key.pin = key_pin;
	key_old_status = key_status_t::KEY_RELEASE;
	key_current_status = key_status_t::KEY_RELEASE;
	key_init();
	
	full_light.rcu_port = full_light_rcu_port;
	full_light.port = full_light_port;
	full_light.pin = full_light_pin;
	full_light.r = 0;
	full_light.g = 255;
	full_light.b = 0;
	full_light_type = full_light_type_t::WS2812B;
	full_light_status = full_light_status_t::OFF;
	full_light_init();
	
	open_state = 0;
	close_state = 0;
}

void garbage_pitch::motor_ctl(motor_cmd_t cmd)
{
	switch(cmd)
	{
		case motor_cmd_t::STOP:
			gpio_bit_reset(motor.port1, motor.pin1);
			gpio_bit_reset(motor.port2, motor.pin2);
			motor_status = motor_status_t::MOTOR_STOP;
			break;
		case motor_cmd_t::OPEN:
			motor_ctl(motor_cmd_t::STOP);
			gpio_bit_reset(motor.port2, motor.pin2);
			gpio_bit_set(motor.port1, motor.pin1);
			motor_status = motor_status_t::MOTOR_OPEN;
			break;
		case motor_cmd_t::CLOSE:
			motor_ctl(motor_cmd_t::STOP);
			gpio_bit_reset(motor.port1, motor.pin1);
			gpio_bit_set(motor.port2, motor.pin2);
			motor_status = motor_status_t::MOTOR_CLOSE;
			break;
		default:
			break;
	}
	
}

void garbage_pitch::motor_init()
{
	rcu_periph_clock_enable(motor.rcu_port1);
	rcu_periph_clock_enable(motor.rcu_port2);
	
	gpio_bit_reset(motor.port1, motor.pin1);
	gpio_bit_reset(motor.port2, motor.pin2);
	
	gpio_init(motor.port1, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, motor.pin1);
	gpio_init(motor.port2, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, motor.pin2);
}

void garbage_pitch::key_init()
{
	rcu_periph_clock_enable(key.rcu_port);
	
	gpio_bit_reset(key.port, key.pin);
	
	gpio_init(key.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, key.pin);
}

int garbage_pitch::key_read_status()
{
	if((key_current_status == key_status_t::KEY_PRESS) && (key_old_status == key_status_t::KEY_PRESS))
	{
		return 1;
	}
	if(gpio_input_bit_get(key.port, key.pin) == RESET)
	{
		key_current_status = key_status_t::KEY_PRESS;
	}
	else
	{
		key_current_status = key_status_t::KEY_RELEASE;
	}
	key_old_status = key_current_status;
	
	return 0;
}

void garbage_pitch::full_light_init()
{
	rcu_periph_clock_enable(full_light.rcu_port);
	
	gpio_bit_reset(full_light.port, full_light.pin);
	
	gpio_init(full_light.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, full_light.pin);
}

void garbage_pitch::full_light_ctl(full_light_type_t full_light_type, uint8_t r, uint8_t g, uint8_t b, full_light_status_t full_light_status)
{
	uint32_t rgb = (g << 24) + (r << 16) + (b << 8);
	switch(full_light_type)
	{
		case full_light_type_t::WS2812B:
			for(int i = 0; i < 16; i++)
			{
				for(int j = 0; j < 24; j ++)
				{
					if(rgb & 0x80000000)
					{
						ws2812b_send_1();
					}
					else
					{
						ws2812b_send_0();
					}
					rgb <<= 1;
				}
				rgb = (g << 24) + (r << 16) + (b << 8);
			}
			break;
		
		case full_light_type_t::RED_LED:
			if(full_light_status == full_light_status_t::ON)
			{
				gpio_bit_set(full_light.port, full_light.pin);
			}
			else
			{
				gpio_bit_reset(full_light.port, full_light.pin);
			}
			break;
		default:
			break;
	}
}

void garbage_pitch::ws2812b_send_0()
{
	gpio_bit_reset(full_light.port, full_light.pin);
	ws2812b_delay(14);
	gpio_bit_set(full_light.port, full_light.pin);
}

void garbage_pitch::ws2812b_send_1()
{
	gpio_bit_reset(full_light.port, full_light.pin);
	ws2812b_delay(4);
	gpio_bit_set(full_light.port, full_light.pin);
	ws2812b_delay(5);
}

void garbage_pitch::ws2812b_delay(uint32_t n)
{
	while(n --);
}

extern volatile uint16_t adc_values[6];

void garbage_pitch::adc_get_benchmark_value()
{
	volatile uint16_t currnet_value = 0;
	
	uint8_t open_finish_count = 0;
	uint32_t open_add_count = 0;
	
	uint8_t close_finish_count = 0;
	uint32_t close_add_count = 0;
	
	motor_ctl(motor_cmd_t::STOP);
	motor_ctl(motor_cmd_t::OPEN);
	vTaskDelay(500);
	
	while(1)
	{
		if(open_finish_count >= 200)
			break;
		
		currnet_value = adc_values[pitch_no];
		
		if(currnet_value < adc_zero + 10)
			open_finish_count ++;
		else
		{
			if(open_finish_count)
				open_finish_count--;
		}
		vTaskDelay(10);
	}
	open_state = 1;
	close_state = 0;
	
	vTaskDelay(500);
	
	motor_ctl(motor_cmd_t::STOP);
	motor_ctl(motor_cmd_t::CLOSE);
	vTaskDelay(500);
	
	while(1)
	{
		if(close_finish_count >= 200)
			break;
		
		currnet_value = adc_values[pitch_no];
		
		if(currnet_value < adc_zero + 10)
			close_finish_count ++;
		else
		{
			if(close_finish_count)
				close_finish_count--;
		}
		vTaskDelay(10);
	}
	
	open_state = 0;
	close_state = 1;
	
	close_finish_count = 0;
	open_finish_count = 0;
	
	vTaskDelay(500);
	
	motor_ctl(motor_cmd_t::STOP);
	motor_ctl(motor_cmd_t::OPEN);
	vTaskDelay(500);

	while(1)
	{
		if(open_finish_count >= 200)
			break;
		
		currnet_value = adc_values[pitch_no];
		
		if(currnet_value < adc_zero + 10)
			open_finish_count ++;
		else
		{
			if(open_finish_count)
				open_finish_count--;
		}
		if(currnet_value > 100)
		{
			open_pitch_benchmark_value += currnet_value;
			open_add_count++;
		}
		vTaskDelay(10);
	}

	if(open_pitch_benchmark_value)
		open_pitch_benchmark_value /= open_add_count;
	
	
	usart_data_transmit(UART3, open_pitch_benchmark_value);
	while(RESET == usart_flag_get(UART3, USART_FLAG_TC));
	
	
	open_state = 1;
	close_state = 0;
	
	vTaskDelay(500);
	
	motor_ctl(motor_cmd_t::STOP);
	motor_ctl(motor_cmd_t::CLOSE);
	vTaskDelay(500);
	
	while(1)
	{
		if(close_finish_count >= 200)
			break;
		
		currnet_value = adc_values[pitch_no];
		
		if(currnet_value < adc_zero + 10)
			close_finish_count ++;
		else
		{
			if(close_finish_count)
				close_finish_count--;
		}
		if(currnet_value > 100)
		{
			close_pitch_benchmark_value += currnet_value;
			close_add_count ++;
		}
		vTaskDelay(10);
	}
	
	if(close_pitch_benchmark_value)
		close_pitch_benchmark_value /= close_add_count;
	
	
	usart_data_transmit(UART3, close_pitch_benchmark_value);
	while(RESET == usart_flag_get(UART3, USART_FLAG_TC));
	
	
	open_state = 0;
	close_state = 1;
	
	motor_ctl(motor_cmd_t::STOP);
	current_init_flag = 1;
}

int garbage_pitch::wait_pitch_open()
{
	volatile int32_t currnet_value = 0;
	uint16_t old_currnet_value = 0;
	
	uint8_t open_finish_count = 0;
	uint8_t error_count = 0;
	
	motor_ctl(motor_cmd_t::STOP);
	motor_ctl(motor_cmd_t::OPEN);
	vTaskDelay(500);
	
	while(1)
	{
		if(open_finish_count >= 200)
			break;
		
		if(error_count >= 20)
		{
			pitch_error_flag = 1;
			motor_ctl(motor_cmd_t::STOP);
			return -1;
		}
		currnet_value = (int32_t)adc_values[pitch_no] - (int32_t)open_pitch_benchmark_value;
		
		if((currnet_value > 1.15*open_pitch_benchmark_value) && (old_currnet_value > 1.15*open_pitch_benchmark_value))
		{
			error_count++;
		}
		else
		{
			if(error_count)
				error_count --;
		}
			
		if(currnet_value < adc_zero + 10)
		{
			open_finish_count ++;
		}
		else
		{
			if(open_finish_count)
				open_finish_count--;
		}
		
		old_currnet_value = currnet_value;
		
		vTaskDelay(10);
	}
	
	open_state = 1;
	close_state = 0;
	return 0;
}

int garbage_pitch::wait_pitch_close()
{
	volatile int32_t currnet_value = 0;
	uint16_t old_currnet_value = 0;
	
	uint8_t close_finish_count = 0;
	uint8_t error_count = 0;
	
	motor_ctl(motor_cmd_t::STOP);
	motor_ctl(motor_cmd_t::CLOSE);
	vTaskDelay(500);
	
	while(1)
	{
		if(close_finish_count >= 200)
			break;
		
		if(error_count >= 20)
		{
			pitch_error_flag = 1;
			motor_ctl(motor_cmd_t::STOP);
			return -1;
		}
		currnet_value = (int32_t)adc_values[pitch_no] - (int32_t)close_pitch_benchmark_value;
		
		if((currnet_value > 1.15*close_pitch_benchmark_value) && (old_currnet_value > 1.15*close_pitch_benchmark_value))
		{
			error_count++;
		}
		else
		{
			if(error_count)
				error_count --;
		}
			
		if(currnet_value < adc_zero + 10)
		{
			close_finish_count ++;
		}
		else
		{
			if(close_finish_count)
				close_finish_count--;
		}
		
		old_currnet_value = currnet_value;
		
		vTaskDelay(10);
	}
	
	open_state = 0;
	close_state = 1;
	
	return 0;
}



