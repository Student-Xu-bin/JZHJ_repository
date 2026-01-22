#ifndef __DEFINES_H__
#define __DEFINES_H__

typedef enum {
    OFF = 0, ON, STOP, REVERSE, ERR
} SWITCH_STATE;

typedef enum {
	IF1 = 0, IF2, IF3, IF4, IF5, IF6
} interface_enum_t;

enum {
	PLC_ADDR = 0, PLC_FUNCTION, PLC_BYTES,
	PLC_DOOR1_IDLE_STATE_H, PLC_DOOR1_IDLE_STATE_L,
	PLC_DOOR2_IDLE_STATE_H, PLC_DOOR2_IDLE_STATE_L,
	PLC_DOOR3_IDLE_STATE_H, PLC_DOOR3_IDLE_STATE_L,
	PLC_DOOR1_WEIGHT_H, PLC_DOOR1_WEIGHT_L,
	PLC_DOOR2_WEIGHT_H, PLC_DOOR2_WEIGHT_L,
	PLC_DOOR3_WEIGHT_H, PLC_DOOR3_WEIGHT_L,
	PLC_FULL_STATE_H, PLC_FULL_STATE_L,
	PLC_ERR_STATE_H, PLC_ERR_STATE_L
};

#define S_ADDR 		0x03

#define OPEN_CLOSE_DOOR_EX                  "C54:"
#define _OPEN_CLOSE_DOOR(door, status)       OPEN_CLOSE_DOOR_EX#door"-"#status";"
#define OPEN_CLOSE_DOOR(door, status)       _OPEN_CLOSE_DOOR(door, status)
#define SEND_OPEN_CLOSE_DOOR()              "C54:OK"

#define READ_OPEN_DOOR_STATE_EX             "C62:"
#define _READ_OPEN_DOOR_STATE(door)         READ_OPEN_DOOR_STATE_EX#door";"
#define READ_OPEN_DOOR_STATE(door)          _READ_OPEN_DOOR_STATE(door)
#define SEND_OPEN_DOOR_STATE()              "C62:OK1"

#define READ_CLOSE_DOOR_STATE_EX            "C63:"
#define _READ_CLOSE_DOOR_STATE(door)        READ_CLOSE_DOOR_STATE_EX#door";"
#define READ_CLOSE_DOOR_STATE(door)         _READ_CLOSE_DOOR_STATE(door)
#define SEND_CLOSE_DOOR_STATE()             "C63:OK0"

#define OPEN_CLOSE_LOCK_EX                  "C55:"
#define _OPEN_CLOSE_LOCK(door, status)      OPEN_CLOSE_LOCK_EX#door"-"#status";"
#define OPEN_CLOSE_LOCK(door, status)       _OPEN_CLOSE_LOCK(door, status)
#define SEND_OPEN_CLOSE_LOCK()              "C55:OK"

#define READ_WEIGHT_EX                      "C52:"
#define _READ_WEIGHT(door)                   READ_WEIGHT_EX#door"-0;"
#define READ_WEIGHT(door)                   _READ_WEIGHT(door)
#define SEND_WEIGHT(z, x)                   "C52:OK"#z"."#x

#define READ_FULL_STATE_EX                  "C100:"
#define _READ_FULL_STATE(door)              READ_WEIGHT_EX#door";"
#define READ_FULL_STATE(door)               _READ_FULL_STATE(door)
//#define SEND_FULL_STATE(dustbin1, dustbin2) "C100:OKT0:"#dustbin1"00-000.00-000;T1:"#dustbin2"00-000.00-000;"
#define SEND_FULL_STATE(dustbin, status) 	"C100:OKT"#dustbin":"#status"00-000.00-000;"
#define SEND_ERR_WEIGHT() 					"C52:OK999.99"

#define KEY1_PRESSED 						"E02:1-1;"
#define KEY2_PRESSED 						"E02:0-1;"

#define SEND_IDLE_STATE()					"C102:OK0"
#define SEND_BUSY_STATE()					"C102:OK1"

#define S_WORK_DOOR1						((uint8_t []){0x03, 0x06, 0x00, 0x00, 0x00, 0x01, 0x49, 0xE8})
#define S_WORK_DOOR2						((uint8_t []){0x03, 0x06, 0x00, 0x01, 0x00, 0x01, 0x18, 0x28})
#define S_WORK_DOOR3						((uint8_t []){0x03, 0x06, 0x00, 0x02, 0x00, 0x01, 0xE8, 0x28})

#define S_READ_DOORS						((uint8_t []){0x03, 0x03, 0x00, 0x00, 0x00, 0x08, 0x45, 0xEE})

#endif
