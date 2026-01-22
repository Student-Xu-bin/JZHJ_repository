#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "gd32f30x.h"                   // Device header


#define FLASH_SIZE                         0x80000
#define APP1_ADDR_IN_FLASH                 0x8010000
#define APP2_ADDR_IN_FLASH                 0x8004000
#define FLASH_APP1_SIZE                    (FLASH_SIZE - (APP1_ADDR_IN_FLASH - 0x08000000))  //计算app1空间可用大小
#define FLASH_APP2_SIZE                    (FLASH_SIZE - (APP2_ADDR_IN_FLASH - 0x08000000))  //计算app2空间可用大小

void UpdateApp(void);

#endif


