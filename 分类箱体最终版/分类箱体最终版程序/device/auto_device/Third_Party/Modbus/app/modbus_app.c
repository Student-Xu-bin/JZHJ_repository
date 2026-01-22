#include <stdint.h>
#include <stdlib.h>
#include "modbus_slave.h"
#include "gd32f30x.h"
#include "74hc595.h"
#include "dev_motor.h"
#include "drv_motor.h"

#define R   (1 << 0)
#define W   (1 << 1)

#define switch_bytes16(x) ((uint16_t)(((x) >> 8) | ((x) << 8)))

typedef struct {
    uint8_t  property;				// 读写的属性
    const uint16_t address;			// 地址
    uint16_t minValue;				// 最小值
    uint16_t maxValue;				// 最大值
	void (*ReadCb)(uint16_t *value);
	void (*WriteCb)(uint16_t value);
} MbRegisterInstance_t;

#define 	DOOR_OPEN_START 				(1 << 0)
#define 	DOOR_CLOSE_START 				(1 << 1)
#define     DOOR_GET_CURRENT				(1 << 2)
#define 	DOOR_ALL_EVENTS					(0x07)

extern uint8_t ir_buf[6];

void read_full_state(uint16_t *val)
{
	*val = 0;
	
	for(int i = 0; i < 6; i ++)
	{
		(*val) |= ir_buf[i];
	}
	
	uint8_t *tmp = (uint8_t *)val;
	
	uint8_t s = tmp[0];
	tmp[0] = tmp[1];
	tmp[1] = s;
}

void read_channel_init(uint16_t *val)
{
	uint8_t temp = 0;

	for(int i = 0; i < 6; i++)
	{
		temp += (motor[i].channel_init_flag << i);
	}
	
	*val = switch_bytes16(temp);
}


void read_open_door_state(uint16_t *val)
{
	*val = 0;
	uint8_t temp_open = 0;
	uint8_t temp_close = 0;
	
	for(int i = 0; i < 6; i ++)
	{
		temp_open += (motor[i].open_state << (5-i));
		temp_close += (motor[i].close_state << (5-i));
	}
	
	*val = (temp_open << 8) + temp_close;
	
	uint8_t *tmp = (uint8_t *)val;
	
	uint8_t s = tmp[0];
	tmp[0] = tmp[1];
	tmp[1] = s;
}

void read_error_state(uint16_t *val)
{
	uint8_t temp = 0;
	
	for(int i = 0; i < 6; i++)
	{
		temp += (motor[i].error_flag << i);
	}
	
	*val = switch_bytes16(temp);
}

static volatile int channel_no = -1;

void write_channel_select(uint16_t val)
{
	channel_no = val - 1;
}

extern TaskHandle_t tasks_motor_current[6];

void write_channel_work(uint16_t val)
{
	if(channel_no < 0)
		return;
	
	switch(val)
	{
	case 4 :
		xTaskNotify(tasks_motor_current[channel_no], DOOR_OPEN_START, eSetBits);
		break;
	case 5 :
		xTaskNotify(tasks_motor_current[channel_no], DOOR_CLOSE_START, eSetBits);
		break;
	case 6 :
		motor_stop(&motor[channel_no]);
		break;
	case 7:
		_74hc595_ctl((enum channel_enum_t)channel_no, RED_LED, OFF, 1);
		break;
	case 8:
		_74hc595_ctl((enum channel_enum_t)channel_no, RED_LED, ON, 1);
		break;
	case 9:
		xTaskNotify(tasks_motor_current[channel_no], DOOR_GET_CURRENT, eSetBits);
		break;
	default:
		break;
	}
	
	channel_no = (-1);
}

void write_led_r(uint16_t val)
{
	if(channel_no < 0)
		return;
	
	extern volatile uint8_t color[6][3];
	
	color[channel_no][0] = val;
}

void write_led_g(uint16_t val)
{
	if(channel_no < 0)
		return;
	
	extern volatile uint8_t color[6][3];
	
	color[channel_no][1] = val;
}

void write_led_b(uint16_t val)
{
	if(channel_no < 0)
		return;
	
	extern volatile uint8_t color[6][3];
	
	color[channel_no][2] = val;
	
	channel_no = (-1);
}

void write_channel_init(uint16_t value)
{
	if(channel_no < 0)
		return;
	motor[channel_no].channel_init_flag = value;
	
	channel_no = (-1);
}

void write_restart(uint16_t value)
{
	__set_FAULTMASK(1);
	
	NVIC_SystemReset();
}

static MbRegisterInstance_t g_regInstanceTab[] = {
	{
		.property = R,
		.address  = 0x000c,
		.ReadCb   = read_full_state, /*  */
	},
	{
		.property = R,
		.address  = 0x000d,
		.ReadCb   = read_channel_init, /*  */
	},
	//motify
	{
		.property = R,
		.address  = 0x000e,
		.ReadCb   = read_error_state,
		
	},
	{
		.property = R,
		.address  = 0x000f,
		.ReadCb   = read_open_door_state,  /* 开关门状态：已开：1，未开：0 */
	},
	{
		.property = W,
		.address  = 0x00f1,
		.minValue = 1,
		.maxValue = 6,
		.WriteCb  = write_channel_select,//00取消
	},
	{
		.property = W,
		.address  = 0x00f2,
		.minValue = 0,
		.maxValue = 0x000b,
		.WriteCb  = write_channel_work,
	},
	{
		.property = W,
		.address  = 0x00f3,
		.minValue = 0,
		.maxValue = 0x00ff,
		.WriteCb  = write_led_r,
	},
	{
		.property = W,
		.address  = 0x00f4,
		.minValue = 0,
		.maxValue = 0x00ff,
		.WriteCb  = write_led_g,
	},
	{
		.property = W,
		.address  = 0x00f5,
		.minValue = 0,
		.maxValue = 0x00ff,
		.WriteCb  = write_led_b,
	},
	{
		.property = W,
		.address  = 0x00f6,
		.minValue = 0,
		.maxValue = 0,
		.WriteCb  = write_restart,
	},
	{
		.property = W,
		.address  = 0x00f7,
		.minValue = 1,
		.maxValue = 6,
		.WriteCb  = write_channel_init,
	},
};

#define REG_TABLE_SIZE         (sizeof(g_regInstanceTab) / sizeof(g_regInstanceTab[0]))


static eMBErrorCode ReadRegsCb(uint8_t startAddr, uint8_t regNum, uint8_t *buf)
{
	if (buf == NULL)
	{
        return MB_EINVAL;
	}
	
    for (int32_t i = 0; i < regNum; i++)
	{
		MbRegisterInstance_t *instance = NULL;
		for (int32_t j = 0; j < REG_TABLE_SIZE; j++)
		{
			if (g_regInstanceTab[j].address != startAddr + i)
			{
				continue;
			}
			instance = &g_regInstanceTab[j];
			if((instance->property & R) == 0)  //读写检验
			{
				return MB_EINVAL;
			}

			if (instance->ReadCb != NULL)
			{
				instance->ReadCb((uint16_t *)&buf[2 * i]);
			}
		}
		if (instance == NULL)
		{
			return MB_ENOREG;
		}
		
    }

    return MB_ENOERR;
}

static eMBErrorCode WriteRegsCb(uint8_t startAddr, uint8_t regNum, uint8_t *buf)
{
    if (buf == NULL)
	{
        return MB_EINVAL;
	}
	
    for (int32_t i = 0; i < regNum; i++)
	{
		
		MbRegisterInstance_t *instance = NULL;
		for (int32_t j = 0; j < REG_TABLE_SIZE; j++)
		{
			if (g_regInstanceTab[j].address != startAddr + i)
			{
				continue;
			}
			instance = &g_regInstanceTab[j];
			
		    if((instance->property & W) == 0)  //读写检验
			{
				return MB_EINVAL;
			}
			
			uint16_t setValue = ((buf[2 * i] << 8) & 0xFF00) | (buf[2 * i + 1] & 0xFF);
			if ((setValue < instance->minValue) || (setValue > instance->maxValue))
			{
				return MB_EINVAL;
			}
			
			if (instance->WriteCb != NULL)
			{
				instance->WriteCb(setValue);
			}
		}
		
		if (instance == NULL)
		{
			return MB_ENOREG;
		}
	}	
	return MB_ENOERR;
}

void ModbusAppInit(void)
{
	ModbusSlaveInstance_t mbInstace = {
        .baudRate = 115200U,
        .cb.ReadRegs = ReadRegsCb,
        .cb.WriteRegs = WriteRegsCb,
        .slaveAddr = 0x0f,
    };
	ModbusSlaveInit(&mbInstace);
}

void ModbusTask(void)
{
	(void)eMBPoll();
}
