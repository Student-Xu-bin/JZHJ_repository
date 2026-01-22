#include <stdio.h>
#include "bootloader.h"
#include "rs485.h"
#include "delay.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "flash_drv.h"

uint32_t jumpaddress = 0;
typedef void(*fun)(void);
fun jump_to_app;

void print_last_app_info(void)
{
	if(strcmp((char*)0x08071000, "app_1") == 0){
		image_header_t *image;
		image = (image_header_t *)0x08070000;
		printf("name:%s\r\n",image->name);
		printf("version:%d.%d.%d\r\n", (image->version % 100)%10, (image->version % 10) %10, image->version % 10);
		switch(image->chip){
		case 1:
			printf("chip:GD32F303\r\n");
			break;
		default:
			break;
		}
		printf("size:%dbytes\r\n",image->size);
		switch(image->os){
		case 1:
			printf("operating system:NOOS\r\n");
			break;
		case 2:
			printf("operating system:FREERTOS\r\n");
			break;
		case 3:
			printf("operating system:RTTHRED\r\n");
			break;
		default:
			break;
		}
		
		printf("load:0x%lx\r\n",(unsigned long)image->load);
	}
	else if(strcmp((char*)0x08071000, "app_2") == 0){
		image_header_t *image;
		image = (image_header_t *)0x08070800;
		printf("name:%s\r\n",image->name);
		printf("version:%d.%d.%d\r\n", (image->version % 100)%10, (image->version % 10) %10, image->version % 10);
		switch(image->chip){
		case 1:
			printf("chip:GD32F303\r\n");
			break;
		default:
			break;
		}
		printf("size:%dbytes\r\n",image->size);
		switch(image->os){
		case 1:
			printf("operating system:NOOS\r\n");
			break;
		case 2:
			printf("operating system:FREERTOS\r\n");
			break;
		case 3:
			printf("operating system:RTTHRED\r\n");
			break;
		default:
			break;
		}
		
		printf("load:0x%lx\r\n",(unsigned long)image->load);
	}	
	else
		printf("last have no app,please do not jump to app,now go to bootloader\r\n");
}

void app_info(char* app)
{
	delay_nms(10);
	if(strcmp(app, "app_1") == 0){
		image_header_t *image;
		image = (image_header_t *)0x08070000;
		printf("====================================================\r\n");
		printf("name:%s\r\n",image->name);
		printf("version:%d.%d.%d\r\n", (image->version % 100)%10, (image->version % 10) %10, image->version % 10);
		switch(image->chip){
		case 1:
			printf("chip:GD32F303\r\n");
			break;
		default:
			break;
		}
		printf("size:%dbytes\r\n",image->size);
		switch(image->os){
		case 1:
			printf("operating system:NOOS\r\n");
			break;
		case 2:
			printf("operating system:FREERTOS\r\n");
			break;
		case 3:
			printf("operating system:RTTHRED\r\n");
			break;
		default:
			break;
		}
		printf("load:0x%lx\r\n",(unsigned long)image->load);
		printf("====================================================\r\n");
	}
	if(strcmp(app, "app_2") == 0){
		image_header_t *image;
		image = (image_header_t *)0x08070800;
		printf("====================================================\r\n");
		printf("name:%s\r\n",image->name);
		printf("version:%d.%d.%d\r\n", (image->version % 100)%10, (image->version % 10) %10, image->version % 10);
		switch(image->chip){
		case 1:
			printf("chip:GD32F303\r\n");
			break;
		default:
			break;
		}
		printf("size:%dbytes\r\n",image->size);
		switch(image->os){
		case 1:
			printf("operating system:NOOS\r\n");
			break;
		case 2:
			printf("operating system:FREERTOS\r\n");
			break;
		case 3:
			printf("operating system:RTTHRED\r\n");
			break;
		default:
			break;
		}
		printf("load:0x%lx\r\n",(unsigned long)image->load);
		printf("====================================================\r\n");
	}
//		printf("Please Choose Your Command:\r\n");
//		printf("1:Updata\r\n");
//		printf("2:query app_1 infomation\r\n");
//		printf("3:query app_2 infomation\r\n");
//		printf("4:Jump to APP_1\r\n");
//		printf("5:Jump to APP_2\r\n");	
//		printf("====================================================\r\n");
}

#define NOOS 		1
#define FREERTOS 	2
#define RTTHRED		3

void jump_to_app_1(void)
{
	image_header_t *image;
	image = (image_header_t *)0x08070000;
	if(image->magic != 0x27052024)
	{
		printf("NO APP1\r\n");
		return;
	}
	
	if(strcmp((char*)0x08071000, "app_1") != 0)
	{
		FlashErase(0x08071000, 2048);
		char app_add[] = {"app_1"};
		FlashWrite(0x08071000, (uint8_t*)app_add, sizeof(app_add));
	}
	
	if(image->os == NOOS)
		__disable_irq();
	else if(image->os == FREERTOS)
		__set_FAULTMASK(1);
	else if(image->os == RTTHRED)
		__set_FAULTMASK(1);
	else{
		printf("EEROR\r\n");
	}
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x10000);
	
	delay_nms(10);
//	printf("\r\ninfomation download...\r\n");
//	printf("jumping...\r\n");
	uint8_t ada5[] = {0xAD, 0xA5};
	ttl_sendbyte(ada5,2);
	
	jumpaddress = *(__IO uint32_t*)(APP_1_BASE_ADDRESSS + 4);
	jump_to_app = (fun)jumpaddress;
	__set_MSP(*(__IO uint32_t*)APP_1_BASE_ADDRESSS);
	jump_to_app();
}

void jump_to_app_2(void)
{
	image_header_t *image;
	image = (image_header_t *)0x08070800;
	
	if(image->magic != 0x27052024)
	{
		printf("NO APP2\r\n");
		return;
	}
	
	if(strcmp((char*)0x08071000, "app_2") != 0)
	{
		FlashErase(0x08071000, 2048);
		char app_add[] = {"app_2"};
		FlashWrite(0x08071000, (uint8_t*)app_add, sizeof(app_add));
	}
	
	if(image->os == NOOS)
		__disable_irq();
	else if(image->os == FREERTOS)
		__set_FAULTMASK(1);
	else if(image->os == RTTHRED)
		__set_FAULTMASK(1);
	else{
		printf("ERROR\r\n");
	}
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x40000);
	
	delay_nms(10);
	
	uint8_t ada5[] = {0xAD, 0xA5};
	ttl_sendbyte(ada5,2);
//	printf("\r\ninfomation download...\r\n");
//	printf("jumping...\r\n");

	jumpaddress = *(__IO uint32_t*)(APP_2_BASE_ADDRESSS + 4);
	jump_to_app = (fun)jumpaddress;
	__set_MSP(*(__IO uint32_t*)APP_2_BASE_ADDRESSS);
	jump_to_app();
}
