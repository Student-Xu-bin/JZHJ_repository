#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <stddef.h>

#define 	GD32F303 				1
#define 	WS2812B 				1
#define 	RED_LIGHT 				2

#define 	JZHJ_MCU 				GD32F303

#define 	JZHJ_FULL_LIGHT 		WS2812B

#define 	CHANNEL_NUMBERS 		6

#define 	WORK_TASK_NUMBERS 		10

#define jzhj_section(x)               __attribute__((section(x)))
#define jzhj_used                     __attribute__((used))
#define jzhj_align(n)                 __attribute__((aligned(n)))
#define jzhj_weak                     __attribute__((weak))
#define jzhj_typeof                   typeof
#define jzhj_noreturn
#define jzhj_inline                   static __inline
#define jzhj_always_inline            jzhj_inline

typedef void ( * init_fn_t)(void);

#define INIT_EXPORT(fn, level)                                             \
            jzhj_used init_fn_t __init_##fn jzhj_section(".rti_fn." level) = fn;

#define INIT_DEVICE_EXPORT(fn)       INIT_EXPORT(fn, "0.1")
#define INIT_DRIVE_EXPORT(fn) 		 INIT_EXPORT(fn, "1.1")

/* LED defines start */

#define 	CMD_LED_CTRL 	0xef

#define 	LED_ON 	 0xf0
#define 	LED_OFF  0xf1

/* LED defines end */

/* MOTOR defines start */

#define 	CMD_MOTOR_ON 	0x01
#define 	CMD_MOTOR_OFF 	0x02
#define 	CMD_MOTOR_STOP  0x03

/* MOTOR defines end */

/* FULL LIGHT defines start */

#define 	CMD_FULL_LIGHT_GREEN 	0x01
#define 	CMD_FULL_LIGHT_RED 		0x02
#define 	CMD_FULL_LIGHT_BLUE  	0x03
#define 	CMD_FULL_LIGHT_YELLOW 	0x04
#define 	CMD_FULL_LIGHT_ON		0xf3
#define 	CMD_FULL_LIGHT_OFF		0x3f

/* FULL LIGHT defines end */

/* BSP LED defines start */

#define 	CMD_BSP_LED_ON		 	0x01
#define 	CMD_BSP_LED_OFF		 	0x02

/* BSP LED defines end */

/* MOTOR DECTET defines start */

#define 	MOTOR_OVERLOAD_COEFFICIENT 	0.15
#define 	MOTOR_CLSOE_ZERO_DRIFT		10
#define 	MOTOR_OPEN_ZERO_DRIFT		10
#define 	MOTOR_ERROR_STOP_TIMES		10
#define 	MOTOR_NO_ERROR_BREAK_TIMES	200

/* MOTOR DECTET defines end */

#ifndef NULL
#define NULL 	(void *)0
#endif /* NULL */

/* note defines start */

#define 	DOOR_OPEN_START 				(1 << 0)
#define 	DOOR_CLOSE_START 				(1 << 1)

#define 	DOOR_ALL_EVENTS					(0x03)

/* note defines end */

typedef enum {
	OFF = 0, ON,
	MOTOR_OPEN,
	MOTOR_CLOSE,
	MOTOR_STOP,
	MOTOR_ERROR,
	LIGHT_ON,
	LIGHT_OFF,
	LIGHT_GREEN,
	LIGHT_RED,
	LIGHT_BLUE,
	LIGHT_YELLOW,
	LIGHT_OTHER,
	LIGHT_ERROR,
	IR_FULL,
	IR_EMPTY,
	IR_ERROR,
	KEY_PRESSED,
	KEY_LOOSE,
} state_enum_t;

typedef struct queue_package_t {
	void (*task_cb)(void *pdev);
	void *data;
}queue_package_t;

#endif /* __CONFIG_H__ */
