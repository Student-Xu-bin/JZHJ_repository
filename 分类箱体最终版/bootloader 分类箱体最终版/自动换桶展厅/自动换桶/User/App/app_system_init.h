#ifndef __APP_SYSTEM_INIT_H__
#define __APP_SYSTEM_INIT_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "device.h"
//#include "task.h"

extern TaskHandle_t tasks_motor_current[];
extern SemaphoreHandle_t uart_ttl_mutex;
extern device_t *full_light[];
extern device_t *motor[];
#if 1 //mmm
extern uint16_t adc_zero[];
#endif

extern QueueHandle_t works_queue;

void app_system_init(void);

#endif /* __APP_SYSTEM_INIT_H__ */
