#ifndef __GARBAGE_PITCH_H
#define __GARBAGE_PITCH_H

extern "C"
{
#include "gd32f30x.h"
#include "FreeRTOS.h"
#include "task.h"
}
class garbage_pitch
{
	public:
		int pitch_no;
		uint8_t pitch_error_flag;
		garbage_pitch() = default;
		garbage_pitch(int no, \
					  uint8_t channel, \
					  rcu_periph_enum motot_rcu_port1, rcu_periph_enum motot_rcu_port2, uint32_t motot_port1, uint32_t motot_port2, uint32_t motot_pin1, uint32_t motot_pin2, \
					  rcu_periph_enum key_rcu_port, uint32_t key_port, uint32_t key_pin, \
					  rcu_periph_enum full_light_rcu_port, uint32_t full_light_port, uint32_t full_light_pin);
	
		uint8_t adc_channel;
		uint16_t adc_zero;
		uint16_t close_pitch_benchmark_value;
		uint16_t open_pitch_benchmark_value;
		uint8_t current_init_flag;
		void adc_get_benchmark_value();
	
		enum class motor_cmd_t : uint8_t{
			STOP 	= 0x00,
			OPEN 	= 0x01,
			CLOSE 	= 0x02,
		};
		enum class motor_status_t : uint8_t{
			MOTOR_STOP 	= 0x00,
			MOTOR_OPEN 	= 0x01,
			MOTOR_CLOSE = 0x02,
		};
		struct motor_t{
			rcu_periph_enum rcu_port1;
			rcu_periph_enum rcu_port2;
			uint32_t port1;
			uint32_t port2;
			uint32_t pin1;
			uint32_t pin2;
		};
		motor_t motor;
		motor_status_t motor_status;
		void motor_ctl(motor_cmd_t cmd);
		void motor_init();
		
		enum class key_status_t : uint8_t{
			KEY_RELEASE = 0x00,
			KEY_PRESS	= 0x01,
		};
		struct key_t{
			rcu_periph_enum rcu_port;
			uint32_t port;
			uint32_t pin;
		};
		key_t key;
		key_status_t key_old_status;
		key_status_t key_current_status;
		void key_init();
		int key_read_status();
		
		enum class full_light_type_t : uint8_t{
			WS2812B = 0x00,
			RED_LED	= 0x01,
		};
		enum class full_light_status_t : uint8_t
		{
			OFF = 0x00,
			ON 	= 0x01,
		};
		struct full_light_t{
			rcu_periph_enum rcu_port;
			uint32_t port;
			uint32_t pin;
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};
		full_light_t full_light;
		full_light_type_t full_light_type;
		full_light_status_t full_light_status;
		void full_light_init();
		void full_light_ctl(full_light_type_t full_light_type, uint8_t r, uint8_t g, uint8_t b, full_light_status_t full_light_status);
		void ws2812b_send_1();
		void ws2812b_send_0();
		void ws2812b_delay(uint32_t n);
		
		int wait_pitch_open();
		int wait_pitch_close();
		uint8_t open_state;
		uint8_t close_state;
		
};



#endif
