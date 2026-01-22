#ifndef __STEPPER_H__
#define __STEPPER_H__

#include "gd32f30x.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdbool.h>
//#include "autochange.h"

typedef enum {STEPPER1 = 0, STEPPER2, STEPPER3, STEPPER4, STEPPER5, STEPPER6, STEPPER_ERR} stepper_enum_t;
typedef enum {STEPPER_DUSTBIN1, STEPPER_DUSTBIN2, STEPPER_DUSTBIN3, STEPPER_DUSTBIN_ANY, STEPPER_DUSTBIN_ERR} dustbin_enum_t;


#define 	STEPPER_ACC_MAX 		4800
#define 	FULL_ACC_MAX 			(STEPPER_ACC_MAX << 1)

#define 	STEPPER_SUBDIVISION 		1600 				// 步进电机细分

#define 	STEPPER1_DIR_PIN 		GPIO_PIN_14
#define 	STEPPER1_DIR_PORT 		GPIOC
#define 	STEPPER1_PUL_PIN 		GPIO_PIN_15
#define 	STEPPER1_PUL_PORT 		GPIOC
#define 	STEPPER1_DIR_PORT_RCU 		RCU_GPIOC
#define 	STEPPER1_PUL_PORT_RCU 		RCU_GPIOC
#define 	STEPPER1_TIMER			TIMER5
#define 	STEPPER1_TIMER_RCU		RCU_TIMER5
#define 	STEPPER1_TIMER_IRQ 		TIMER5_IRQn
#define 	STEPPER1_TIMER_HANDLER 		TIMER5_IRQHandler 

#define 	STEPPER2_DIR_PIN 		GPIO_PIN_2
#define 	STEPPER2_DIR_PORT 		GPIOE
#define 	STEPPER2_PUL_PIN 		GPIO_PIN_3
#define 	STEPPER2_PUL_PORT 		GPIOE
#define 	STEPPER2_DIR_PORT_RCU 		RCU_GPIOE
#define 	STEPPER2_PUL_PORT_RCU 		RCU_GPIOE
#define 	STEPPER2_TIMER			TIMER7
#define 	STEPPER2_TIMER_RCU		RCU_TIMER7
#define 	STEPPER2_TIMER_IRQ 		TIMER7_UP_IRQn
#define 	STEPPER2_TIMER_HANDLER 		TIMER7_UP_IRQHandler

#define 	STEPPER3_DIR_PIN 		GPIO_PIN_6
#define 	STEPPER3_DIR_PORT 		GPIOB
#define 	STEPPER3_PUL_PIN 		GPIO_PIN_7
#define 	STEPPER3_PUL_PORT 		GPIOB
#define 	STEPPER3_DIR_PORT_RCU 		RCU_GPIOB
#define 	STEPPER3_PUL_PORT_RCU 		RCU_GPIOB
#define 	STEPPER3_TIMER			TIMER6
#define 	STEPPER3_TIMER_RCU		RCU_TIMER6
#define 	STEPPER3_TIMER_IRQ 		TIMER6_IRQn
#define 	STEPPER3_TIMER_HANDLER 		TIMER6_IRQHandler

#define 	STEPPER4_DIR_PIN 		GPIO_PIN_6
#define 	STEPPER4_DIR_PORT 		GPIOC
#define 	STEPPER4_PUL_PIN 		GPIO_PIN_12
#define 	STEPPER4_PUL_PORT 		GPIOA
#define 	STEPPER4_DIR_PORT_RCU 		RCU_GPIOC
#define 	STEPPER4_PUL_PORT_RCU 		RCU_GPIOA
#define 	STEPPER4_TIMER			TIMER1
#define 	STEPPER4_TIMER_RCU		RCU_TIMER1
#define 	STEPPER4_TIMER_IRQ 		TIMER1_IRQn
#define 	STEPPER4_TIMER_HANDLER 		TIMER1_IRQHandler

#define 	STEPPER5_DIR_PIN 		GPIO_PIN_10
#define 	STEPPER5_DIR_PORT 		GPIOD
#define 	STEPPER5_PUL_PIN 		GPIO_PIN_15
#define 	STEPPER5_PUL_PORT 		GPIOD
#define 	STEPPER5_DIR_PORT_RCU 		RCU_GPIOD
#define 	STEPPER5_PUL_PORT_RCU 		RCU_GPIOD
#define 	STEPPER5_TIMER			TIMER2
#define 	STEPPER5_TIMER_RCU		RCU_TIMER2
#define 	STEPPER5_TIMER_IRQ 		TIMER2_IRQn
#define 	STEPPER5_TIMER_HANDLER 		TIMER2_IRQHandler

#define 	STEPPER6_DIR_PIN 		GPIO_PIN_12
#define 	STEPPER6_DIR_PORT 		GPIOB
#define 	STEPPER6_PUL_PIN 		GPIO_PIN_9
#define 	STEPPER6_PUL_PORT 		GPIOD
#define 	STEPPER6_DIR_PORT_RCU 		RCU_GPIOB
#define 	STEPPER6_PUL_PORT_RCU 		RCU_GPIOD
#define 	STEPPER6_TIMER			TIMER4
#define 	STEPPER6_TIMER_RCU		RCU_TIMER4
#define 	STEPPER6_TIMER_IRQ 		TIMER4_IRQn
#define 	STEPPER6_TIMER_HANDLER 		TIMER4_IRQHandler

enum stepper_state_enum_t
{
    STEPPER_INCREASE,
    STEPPER_STABLE,
    STEPPER_DECREASE,
    STEPPER_IDLE
};

typedef enum {STEPPER_ZHENG, STEPPER_FAN} stepper_dir_enum_t;

typedef struct
{
    uint32_t        dir_gpio;
    uint32_t        pul_gpio;
    uint32_t        dir_pin;
    uint32_t        pul_pin;

    rcu_periph_enum dir_gpio_rcu;
    rcu_periph_enum pul_gpio_rcu;
	
	uint32_t timer_periph;
    rcu_periph_enum timer_rcu;
    uint8_t timer_irq;
} stepper_hw_info_t;

typedef struct {
    stepper_hw_info_t 				stepper_hw_info;    // 硬件信息
    enum stepper_state_enum_t 		status;   			// 状态
    int32_t 						step_count;     	// 加减速过程步数计数
    int32_t 						acc_step_max;  		// 最大大加步数
    uint32_t 						target_step; 		// 目标步数
	
	SemaphoreHandle_t 				idle_semaphore_handle;
} stepper_t;

void stepper_calculate_S(uint16_t *pbuff, uint16_t fre_max, uint16_t fre_min, int16_t len);
//void stepper_set_frequency(stepper_t *stp);
void stepper_init(stepper_t *sp);
//void stepper_work(stepper_enum_t no, dustbin_enum_t dustbin_no);
//void stepper_work(stepper_enum_t no, uint32_t disc);
//void stepper_work(autochange_t *autochange, uint32_t disc);
//stepper_t *stepper_get_stepper(stepper_enum_t no);

#endif
