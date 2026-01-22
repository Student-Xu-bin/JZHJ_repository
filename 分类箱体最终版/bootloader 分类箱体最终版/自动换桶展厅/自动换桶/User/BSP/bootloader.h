#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "gd32f30x.h"

#define APP_1_BASE_ADDRESSS			((uint32_t)0x08010000)	
#define APP_2_BASE_ADDRESSS			((uint32_t)0x08040000)	

#define CURRENT_APP_ADDRESS			((uint32_t)0x08071000)

#define APP_1_SYSTEM_INFORMATION_ADDRESS	((uint32_t)0x08070000)
#define APP_1_VERSION_NUMBER			((uint32_t)0x08070080)
#define APP_1_APP_SIZE				((uint32_t)0x08070100)
#define APP_1_DATE_OF_DOWNLOAD			((uint32_t)0x08070180)
#define APP_1_OPERATING_SYSTEM			((uint32_t)0x08070200)

#define APP_2_SYSTEM_INFORMATION_ADDRESS	((uint32_t)0x08070800)
#define APP_2_VERSION_NUMBER			((uint32_t)0x08070880)
#define APP_2_APP_SIZE				((uint32_t)0x08070900)
#define APP_2_DATE_OF_DOWNLOAD			((uint32_t)0x08070980)
#define APP_2_OPERATING_SYSTEM			((uint32_t)0x08070A00)

#define APP_1_FLAG				(0x31005000)
#define APP_2_FLAG				(0x32005000)
#define BARE_METAL_SYSTEM			(0x61004200)
#define FREERTOS_SYSTEM				(0x72004600)

typedef struct{
	uint32_t magic;
	uint32_t size;
	uint32_t load;
	uint32_t version;
	uint8_t chip;
	uint8_t os;
	char name[20];	
}image_header_t;


void print_last_app_info(void);
void app_info(char* app);
void jump_to_app_1(void);
void jump_to_app_2(void);

#endif
