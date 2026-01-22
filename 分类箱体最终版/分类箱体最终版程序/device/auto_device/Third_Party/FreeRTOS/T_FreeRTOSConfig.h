#ifndef __T_FREERTOSCONFIG_H__
#define __T_FREERTOSCONFIG_H__

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)

#include <stdint.h>
#include <stdio.h>

extern uint32_t SystemCoreClock;

#endif
// 断言
#define vAssertCalled(char, int) 			printf("Error:%s, %d\r\n", char, int)
#define configASSERT(x) 					if((x) == 0) vAssertCalled(__FILE__, __LINE__)


#define configUSE_PREEMPTION 						1
#define configUSE_TIME_SLICING 						1

#define configUSE_PORT_OPTIMISED_TASK_SELECTION 	1
#define configUSE_TICKLESS_IDLE						0

#define configCPU_CLOCK_HZ 							( (uint32_t)(120000000) )
//1ms
#define configTICK_RATE_HZ 							( (TickType_t)1000 )
//可使用最大优先级
#define configMAX_PRIORITIES 						(32)
#define configMINIMAL_STACK_SIZE 					( (unsigned short) 128 )
#define configMAX_TASK_NAME_LE 						(16)
//系统节拍技术及变量的数据类型：1-->16位无符号, 0-->32位无符号
#define configUSE_16_BIT_TICKS						0
//取为0,或者可以为1，但是其他任务的优先级设定比空闲任务搞即可
#define configIDLE_SHOULD_YIELD 					0

#define configUSE_QUEUE_SETS 						1
#define configUSE_TASK_NOTIFICATIONS 				1
#define configUSE_MUTEXES 							1
#define configUSE_RECURSIVE_MUTEXES 				1
#define configUSE_COUNTING_SEMAPHORES 				1
//可以注册的信号量和消息队列的个数
#define configQUEUE_REGISTRY_SIZE 					20
//任务函数取标签
#define configUSE_APPLICATION_TASK_TAG 				0

#define configSUPPORT_DYNAMIC_ALLOCATION 			1
#define configSUPPORT_STATIC_ALLOCATION 			0
//系统总的栈大小
#define configTOTAL_HEAP_SIZE 						( (size_t) (30 * 1024) )

//空闲任务钩子
#define configUSE_IDLE_HOOK 						0
//时间片钩子
#define configUSE_TICK_HOOK 						0
//内存申请失败钩子
#define configUSE_MALLOC_FAILED_HOOK 				0
//栈溢出检测：0,1,2, 如果使用需要提供栈溢出钩子函数
#define configCHECK_FOR_STACK_OVERFLOW 				0

//运行时间统计功能
#define configGENERATE_RUN_TIME_STATS 				0
//可视化跟踪调试
//下面两个宏同时为1时，会编译以下三个函数：
//prvWriteNameToBuffer();
//vTaskList();
//vTaskGetRunTimeStats();
#define configUSE_TRACE_FACILITY 					0
#define configUSE_STATS_FORMATTING_FUNCTIONS 		0

//启用协程 croutine.c
#define configUSE_CO_ROUTINES 						0
//协程的有效优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES 			( 2 )

//启用软件定时器
#define configUSE_TIMERS 							1
#define configTIMER_TASK_PRIORITY 					( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH 					10
#define configTIMER_TASK_STACK_DEPTH 				( configMINIMAL_STACK_SIZE * 2 )

#define INCLUDE_xTaskGetSchedulerState 				1
#define INCLUDE_vTaskPrioritySet 					1
#define INCLUDE_uxTaskPriorityGet 					1
#define INCLUDE_vTaskDelete 						1
#define INCLUDE_vTaskCleanUpResources 				1
#define INCLUDE_vTaskSuspend 						1
#define INCLUDE_vTaskDelayUntil 					1
#define INCLUDE_vTaskDelay 							1
#define INCLUDE_eTaskGetState						1
#define INCLUDE_xTimerPendFunctionCall				1

#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS 							__NVIC_PRIO_BITS
#else
#define configPRIO_BITS 							4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 	15

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configKERNEL_INTERRUPT_PRIORITY 			( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 		( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define xPortPendSVHandler 							PendSV_Handler
#define vPortSVCHandler 							SVC_Handler
//#define xPortSysTickHandler 						SysTick_Handler

#if ( configUSE_TRACE_FACILITY == 1 )

#include "trcRecorder.h"

#define INCLUDE_xTaskGetCurrentTaskHandle 			1

#endif

#endif /* __T_FREERTOSCONFIG_H__ */
