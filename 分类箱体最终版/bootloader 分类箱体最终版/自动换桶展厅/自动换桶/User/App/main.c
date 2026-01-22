#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "rs485.h"
#include "delay.h"
#include "bootloader.h"
#include "update.h"
#include "74hc595.h"
#include "string.h"

int main(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	
	GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_3;
	GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_4;
	
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3 | GPIO_PIN_4);
//	gpio_bit_reset(GPIOB, GPIO_PIN_3 | GPIO_PIN_4);
	
	
	delay_init();
	_74hc595_init();
	
	rs485_init();
	
	uint8_t key_value_of_command; 
	uint8_t key_value_of_system; 
	uint8_t time_out_flag;
	int time = 0;

	uint8_t ada4[2] = {0xAD, 0xA4};
	ttl_sendbyte(ada4, 2);

	printf("\r\n");
	while(1){
		time++;
		if(time %1000000 == 0 && time <10000001)
			printf("%d\r",time/1000000);
		if(rs485_read_byte(&key_value_of_system))
		{
			if(key_value_of_system == 0xe5)	
				break;
		}
		if(time > 10000002){
			time_out_flag = 1;
			break;
		}
	}
	while(1)
	{
		delay_nms(10);
//		printf("\r\n");
		
		if(time_out_flag == 1){
			if(strcmp((char*)0x08071000, "app_1") == 0)
				jump_to_app_1();
			else if(strcmp((char*)0x08071000, "app_2") == 0)
				jump_to_app_2();
			else{
				printf("Enter bootloader\r\n");
				//while(1);
			}		
		}
		delay_nms(10);
		if(key_value_of_system == '9'){
			printf("Please Choose Your Command:\r\n");
			printf("1:Updata\r\n");
			printf("2:query last app infomation\r\n");
			printf("3:query app_1 infomation\r\n");
			printf("4:query app_2 infomation\r\n");
			printf("5:Jump to APP_1\r\n");
			printf("6:Jump to APP_2\r\n");
		}
		while(!rs485_read_byte(&key_value_of_command));
		
		if(key_value_of_command == 0xe1)
			UpdateApp();
		else if(key_value_of_command == 0xe2)
			print_last_app_info();
		else if(key_value_of_command == 0xe3)
			app_info("app_1");
		else if(key_value_of_command == 0xe4)
			app_info("app_2");
		else if(key_value_of_command == 0xe5)
			jump_to_app_1();
		else if(key_value_of_command == 0xe6)
			jump_to_app_2();
		else
			printf("EEROR\r\n");
	}
}
