#include <stdint.h>
#include <stdlib.h>
#include "modbus_slave.h"
#include "app_system_init.h"
#include "app_tasks.h"
#include "queue.h"

#define R   (1 << 0)
#define W   (1 << 1)
	
typedef struct {
    uint8_t  property;				// 读写的属性
    const uint16_t address;			// 地址
    uint16_t minValue;				// 最小值
    uint16_t maxValue;				// 最大值
	void (*ReadCb)(uint16_t *value);
	void (*WriteCb)(uint16_t value);
} MbRegisterInstance_t;

void read_full_state(uint16_t *val)
{
	*val = 0;
	
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
	{
		(*val) |= (app_task_get_full_state(CHANNEL_NUMBERS - 1 - i) << (CHANNEL_NUMBERS - i - 1));
	}
	
	uint8_t *tmp = (uint8_t *)val;
	
	uint8_t s = tmp[0];
	tmp[0] = tmp[1];
	tmp[1] = s;
}

void read_open_door_state(uint16_t *val)
{
	*val = 0;
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
	{
		(*val) |= (app_task_get_open_state(CHANNEL_NUMBERS - 1 - i) << (5 + CHANNEL_NUMBERS - i));
		(*val) |= (app_task_get_close_state(CHANNEL_NUMBERS - 1 - i) << (CHANNEL_NUMBERS - i - 1));
	}
	
	uint8_t *tmp = (uint8_t *)val;
	
	uint8_t s = tmp[0];
	tmp[0] = tmp[1];
	tmp[1] = s;
}


static volatile int channel_no = -1;
void write_channel_select(uint16_t val)
{
	channel_no = val - 1;
}

void write_channel_work(uint16_t val)
{
	if(channel_no < 0)
		return;
	
	switch(val)
	{
	case 4 :
		//motor_open(motor[channel_no]);
		xTaskNotify(tasks_motor_current[channel_no], DOOR_OPEN_START, eSetBits);
		break;
	case 5 :
		//motor_close(motor[channel_no]);
		xTaskNotify(tasks_motor_current[channel_no], DOOR_CLOSE_START, eSetBits);
		break;
	case 6 :
		gpio_bit_reset(motor[channel_no]->hw_info[0].port, motor[channel_no]->hw_info[0].pin);
		gpio_bit_reset(motor[channel_no]->hw_info[1].port, motor[channel_no]->hw_info[1].pin);
		break;
#if (JZHJ_FULL_LIGHT == WS2812B)
	case 7:
		full_light[channel_no]->state = LIGHT_GREEN;
		break;
	case 8:
		full_light[channel_no]->state = LIGHT_RED;
		break;
	case 9:
		full_light[channel_no]->state = LIGHT_BLUE;
		break;
	case 10:
		full_light[channel_no]->state = LIGHT_YELLOW;
		break;
#elif (JZHJ_FULL_LIGHT == RED_LIGHT)
	case 11:
		full_light[channel_no]->state = LIGHT_OFF;
		break;
	case 12:
		full_light[channel_no]->state = LIGHT_ON;
		break;
#endif
	default:
		break;
	}
	
	channel_no = (-1);
}

static MbRegisterInstance_t g_regInstanceTab[] = {
	{
		.property = R,
		.address  = 0x000c,
		.ReadCb   = read_full_state, /*  */
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
		.maxValue = 0x0b,
		.WriteCb  = write_channel_work,
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
