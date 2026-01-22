#include <stdint.h>
#include <stdlib.h>
#include "modbus_slave.h"
#include "FreeRTOS.h"
#include "create_task.h"
#include "rs485.h"
#include "semphr.h"
//#include "stepper.h"
#include "channel.h"

#define R   (1 << 0)
#define W   (1 << 1)

static enum channel_enum_t channel_no = CHANNEL_EMPTY;

typedef struct {
    uint8_t  property;				// 读写的属性
    const uint16_t address;			// 地址
    uint16_t minValue;				// 最小值
    uint16_t maxValue;				// 最大值
	void (*ReadCb)(uint16_t *value);
	void (*WriteCb)(uint16_t value);
} MbRegisterInstance_t;

static void write_channel_select(uint16_t value)
{
	if(value >= 1 && value <= 6)
		channel_no = (enum channel_enum_t)(value - 1);
	else if(value == 0)
		channel_no = CHANNEL_EMPTY;
}
extern TaskHandle_t shouzhen_task_;
static void write_channel_work(uint16_t value)
{
	if(channel_no == CHANNEL_EMPTY)
		return;
	
	switch(value)
	{
	case 0 :
		xTaskNotify(channel[(int)channel_no].task_handle, STEPPER_THROW_1, eSetBits);
		break;
	case 1 :
		xTaskNotify(channel[(int)channel_no].task_handle, STEPPER_THROW_2, eSetBits);
		break;
	case 2 :
		xTaskNotify(channel[(int)channel_no].task_handle, STEPPER_THROW_3, eSetBits);
		break;
	case 3 :
		xTaskNotify(channel[(int)channel_no].task_handle, STEPPER_THROW_AUTO, eSetBits);
		break;
	case 4 : // 开投口
		xTaskNotify(channel[(int)channel_no].task_handle, DOOR_OPEN, eSetBits);
		break;
	case 5 : // 关投口
		xTaskNotify(channel[(int)channel_no].task_handle, DOOR_CLOSE, eSetBits);
		break;
	case 6 : // 停投口
		xTaskNotify(channel[(int)channel_no].task_handle, DOOR_STOP, eSetBits);
		break;
	case 7 :
		xTaskNotify(channel[(int)channel_no].task_handle, _BAGS_BROKEN_READY, eSetBits);
		break;
	case 8 :
		xTaskNotify(channel[(int)channel_no].task_handle, _BAGS_BROKEN_RESET, eSetBits);
		break;
	case 9 :
		xTaskNotify(channel[(int)channel_no].task_handle, _BAGS_BROKEN_WORK, eSetBits);
		break;
	case 10 :
		xTaskNotify(channel[(int)channel_no].task_handle, GS_ENABLE, eSetBits);
		break;
	case 11 :
		xTaskNotify(channel[(int)channel_no].task_handle, GS_DISABLE, eSetBits);
		break;
	case 12 :
		xTaskNotify(channel[(int)channel_no].task_handle, STEPPER_THROW_4, eSetBits);
		break;
	default :
		break;
	}
	
	channel_no = CHANNEL_EMPTY;
}

static void write_full_detect(uint16_t value)
{
	switch(value)
	{
	case 0 : // 通道一满桶检测
		xTaskNotify(channel[0].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	case 1 : // 通道二满桶检测
		xTaskNotify(channel[1].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	case 2 : // 通道三满桶检测
		xTaskNotify(channel[2].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	case 3 : // 通道四满桶检测
		xTaskNotify(channel[3].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	case 4 : // 通道五满桶检测
		xTaskNotify(channel[4].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	case 5 : // 通道六满桶检测
		xTaskNotify(channel[5].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	default :
		xTaskNotify(channel[0].task_handle, STEPPER_FULL_DETECT, eSetBits);
		xTaskNotify(channel[1].task_handle, STEPPER_FULL_DETECT, eSetBits);
		xTaskNotify(channel[2].task_handle, STEPPER_FULL_DETECT, eSetBits);
		xTaskNotify(channel[3].task_handle, STEPPER_FULL_DETECT, eSetBits);
		xTaskNotify(channel[4].task_handle, STEPPER_FULL_DETECT, eSetBits);
		xTaskNotify(channel[5].task_handle, STEPPER_FULL_DETECT, eSetBits);
		break;
	}
}

extern rs485_t slave_rs485;
static volatile uint32_t read_weight_ch1 = 0xffffffff;
static volatile uint32_t read_weight_ch2 = 0xffffffff;
static volatile uint32_t read_weight_ch3 = 0xffffffff;
static volatile uint32_t read_weight_ch4 = 0xffffffff;
static volatile uint32_t read_weight_ch5 = 0xffffffff;
static volatile uint32_t read_weight_ch6 = 0xffffffff;

static void read_channel_1_full_state(uint16_t *value)
{
	uint16_t tmp_count = 0, tmp_state = 0;
	
	taskENTER_CRITICAL();
	
	tmp_state = channel[0].autochange.full_state;
	
	taskEXIT_CRITICAL();
	for(int i = 0; i < DUSTBIN_NUMBERS; i += 1)
	{
		if(tmp_state & 1)
			tmp_count += 1;
		tmp_state >>= 1;
	}
	
	*value = tmp_count;
}

static void read_channel_2_full_state(uint16_t *value)
{
	uint8_t tmp_count = 0, tmp_state = 0;
	
	taskENTER_CRITICAL();
	
	tmp_state = channel[1].autochange.full_state;
	
	taskEXIT_CRITICAL();
	for(int i = 0; i < DUSTBIN_NUMBERS; i += 1)
	{
		if(tmp_state & 1)
			tmp_count += 1;
		tmp_state >>= 1;
	}
	
	*value = tmp_count;
}

static void read_channel_3_full_state(uint16_t *value)
{
	uint8_t tmp_count = 0, tmp_state = 0;
	
	taskENTER_CRITICAL();
	
	tmp_state = channel[2].autochange.full_state;
	
	taskEXIT_CRITICAL();
	for(int i = 0; i < DUSTBIN_NUMBERS; i += 1)
	{
		if(tmp_state & 1)
			tmp_count += 1;
		tmp_state >>= 1;
	}
	
	*value = tmp_count;
}

static void read_channel_4_full_state(uint16_t *value)
{
	uint8_t tmp_count = 0, tmp_state = 0;
	
	taskENTER_CRITICAL();
	
	tmp_state = channel[3].autochange.full_state;
	
	taskEXIT_CRITICAL();
	for(int i = 0; i < DUSTBIN_NUMBERS; i += 1)
	{
		if(tmp_state & 1)
			tmp_count += 1;
		tmp_state >>= 1;
	}
	
	*value = tmp_count;
}

static void read_channel_5_full_state(uint16_t *value)
{
	uint8_t tmp_count = 0, tmp_state = 0;
	
	taskENTER_CRITICAL();
	
	tmp_state = channel[4].autochange.full_state;
	
	taskEXIT_CRITICAL();
	for(int i = 0; i < DUSTBIN_NUMBERS; i += 1)
	{
		if(tmp_state & 1)
			tmp_count += 1;
		tmp_state >>= 1;
	}
	
	*value = tmp_count;
}

static void read_channel_6_full_state(uint16_t *value)
{
	uint8_t tmp_count = 0, tmp_state = 0;
	
	taskENTER_CRITICAL();
	
	tmp_state = channel[5].autochange.full_state;
	
	taskEXIT_CRITICAL();
	for(int i = 0; i < DUSTBIN_NUMBERS; i += 1)
	{
		if(tmp_state & 1)
			tmp_count += 1;
		
		tmp_state >>= 1;
	}
	
	*value = tmp_count;
}

static void read_weight_ch4_l(uint16_t *value)
{
	rs485_write(&slave_rs485, (uint8_t[]){0x01, 0x03, 0x00, 0x06, 0x00, 0x02, 0x24, 0x0a}, 8);
	
	BaseType_t 	ret = pdTRUE;
	uint32_t read_event = 0;
	read_weight_ch4 = 0xffffffff;
	
	ret = xTaskNotifyWait(0, 0xffffffff, &read_event, 100);
	
	if(ret == pdTRUE)
	{
		read_weight_ch4 = read_event;
	}

	*value = (read_weight_ch4 & 0xffff);
}

static void read_weight_ch4_h(uint16_t *value)
{
	if(read_weight_ch4 == 0xffffffff)
	{
		*value = 0xffff;
		return;
	}

	*value = ((read_weight_ch4 >> 16) & 0xffff);
	read_weight_ch4 = 0xffffffff;
}

static void read_weight_ch5_l(uint16_t *value)
{
	rs485_write(&slave_rs485, (uint8_t[]){0x01, 0x03, 0x00, 0x08, 0x00, 0x02, 0x45, 0xc9}, 8);
	
	BaseType_t 	ret = pdTRUE;
	uint32_t read_event = 0;
	read_weight_ch5 = 0xffffffff;
	
	ret = xTaskNotifyWait(0, 0xffffffff, &read_event, 100);
	
	if(ret == pdTRUE)
	{
		read_weight_ch5 = read_event;
	}

	*value = (read_weight_ch5 & 0xffff);
}

static void read_weight_ch5_h(uint16_t *value)
{
	if(read_weight_ch5 == 0xffffffff)
	{
		*value = 0xffff;
		return;
	}

	*value = ((read_weight_ch5 >> 16) & 0xffff);
	read_weight_ch5 = 0xffffffff;
}

static void read_weight_ch6_l(uint16_t *value)
{
	rs485_write(&slave_rs485, (uint8_t[]){0x01, 0x03, 0x00, 0x0a, 0x00, 0x02, 0xe4, 0x09}, 8);
	
	BaseType_t 	ret = pdTRUE;
	uint32_t read_event = 0;
	read_weight_ch6 = 0xffffffff;
	
	ret = xTaskNotifyWait(0, 0xffffffff, &read_event, 100);
	
	if(ret == pdTRUE)
	{
		read_weight_ch6 = read_event;
	}

	*value = (read_weight_ch6 & 0xffff);
}

static void read_weight_ch6_h(uint16_t *value)
{
	if(read_weight_ch6 == 0xffffffff)
	{
		*value = 0xffff;
		return;
	}

	*value = ((read_weight_ch6 >> 16) & 0xffff);
	read_weight_ch6 = 0xffffffff;
}

static uint32_t full_state_mb = 0;

static void read_full_state_l(uint16_t *value)	// 前五个投口
{
	uint8_t full_state[SYSTEM_CHANNEL_NUMBERS] = {0x00};
	full_state_mb = 0;
	
	taskENTER_CRITICAL();

	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		full_state[i] = channel[i].autochange.full_state;
	}	
	
	taskEXIT_CRITICAL();
	
	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		full_state_mb <<= 3;
		full_state_mb |= (full_state[i] & 0x07);
	}
//	full_state_mb = ((full_state[0] & 0x07) << 15) | ((full_state[1] & 0x07) << 12) | ((full_state[2] & 0x07) << 9) |
//					 (full_state[3] & 0x07) << 15) | ((full_state[4] & 0x07) << 12) | ((full_state[5] & 0x07) << 9))
	
	*value = (full_state_mb & 0x7fff);
}

static void read_full_state_h(uint16_t *value) // 第六个投口
{
//#if SYSTEM_CHANNEL_NUMBERS <= 3
//	*value = 0;
//#else
//	uint8_t full_state[3] = {0x00, 0x00, 0x00};
//	
//	taskENTER_CRITICAL();
//	
//	for(int i = 3; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
//	{
//		full_state[i - 3] = channel[i].autochange.full_state;
//	}
//	
//	taskEXIT_CRITICAL();
//	
//	*value = ((full_state[0] & 0x07) << 6) | ((full_state[1] & 0x07) << 3) | (full_state[2] & 0x07);
//#endif
	*value = (full_state_mb >> 15);
}

static void read_bags_broken_state(uint16_t *value) // 0空闲，1忙，2已准备
{
	enum bags_broken_state_enum_t state[3] = {BAGS_BROKEN_IDLE, BAGS_BROKEN_IDLE, BAGS_BROKEN_IDLE};
	
	taskENTER_CRITICAL();
	
	for(int i = 0, j = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		if(channel[i].bags_broken != NULL && channel[i].bags_broken != (void *)-1)
			state[j ++] = bags_broken_get_state(channel[i].bags_broken);
	}
	
	taskEXIT_CRITICAL();
	
	/* 破袋1 | 破袋2 | 破袋3 */
	*value = (((uint8_t)state[0] & 0x03) << 4) | (((uint8_t)state[1] & 0x03) << 2) | (((uint8_t)state[2] & 0x03) << 0);
}

static void read_door_open_state(uint16_t *value)
{
	*value = 0;
	
	taskENTER_CRITICAL();
	
	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		if(cd4051_detect((enum channel_enum_t)i, DOOR_OPEN_DET) == SET)
			(*value) |= (1 << (6 - 1 - i));
	}
	
	taskEXIT_CRITICAL();
}

static void read_door_close_state(uint16_t *value)
{
	*value = 0;
	
	taskENTER_CRITICAL();
	
	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		if(cd4051_detect((enum channel_enum_t)i, DOOR_CLOSE_DET) == SET)
			(*value) |= (1 << (6 - 1 - i));
	}
	
	taskEXIT_CRITICAL();
}

static void read_autochange_work_state(uint16_t *value)
{
	*value = 0;
	
	taskENTER_CRITICAL();
	
	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		if(cd4051_detect((enum channel_enum_t)i, STEPPER_ZERO_DET) == RESET) // 不在零位一律视为在工作
			(*value) |= (1 << (6 - 1 - i));
	}
	
	taskEXIT_CRITICAL();
}

static void read_bags_broken_belongs_to(uint16_t *value)
{
	*value = 0;
	uint8_t count = 0;
	
	taskENTER_CRITICAL();
	
	for(int i = 0; i < SYSTEM_CHANNEL_NUMBERS; i += 1)
	{
		if((channel[i].bags_broken != NULL) && (channel[i].bags_broken != (void *)-1))
		{
			(*value) |= (((uint16_t)(channel[i].no + 1) & 0xf) << (8 - (count << 2)));
			count ++;
			
			if(count >= 3)
				break;
		}
	}
	
	taskEXIT_CRITICAL();
}

static void write_autochange_zero(uint16_t value)
{
	switch(value)
	{
	case 0 : // 通道一回零
		xTaskNotify(channel[0].task_handle, STEPPER_ZERO, eSetBits);
		break;
	case 1 : // 通道二回零
		xTaskNotify(channel[1].task_handle, STEPPER_ZERO, eSetBits);
		break;
	case 2 : // 通道三回零
		xTaskNotify(channel[2].task_handle, STEPPER_ZERO, eSetBits);
		break;
	case 3 : // 通道四回零
		xTaskNotify(channel[3].task_handle, STEPPER_ZERO, eSetBits);
		break;
	case 4 : // 通道五回零
		xTaskNotify(channel[4].task_handle, STEPPER_ZERO, eSetBits);
		break;
	case 5 : // 通道六回零
		xTaskNotify(channel[5].task_handle, STEPPER_ZERO, eSetBits);
		break;
	default :
		xTaskNotify(channel[0].task_handle, STEPPER_ZERO, eSetBits);
		xTaskNotify(channel[1].task_handle, STEPPER_ZERO, eSetBits);
		xTaskNotify(channel[2].task_handle, STEPPER_ZERO, eSetBits);
		xTaskNotify(channel[3].task_handle, STEPPER_ZERO, eSetBits);
		xTaskNotify(channel[4].task_handle, STEPPER_ZERO, eSetBits);
		xTaskNotify(channel[5].task_handle, STEPPER_ZERO, eSetBits);
		break;
	}
}

static void write_man_led_ctl(uint16_t value)
{
	switch(value)
	{
	case 0 :
		man_led_ctl(MAN_LED_GREEN, ON);
		break;
	case 1 :
		man_led_ctl(MAN_LED_GREEN, OFF);
		break;
	case 2 :
		man_led_ctl(MAN_LED_RED, ON);
		break;
	case 3 :
		man_led_ctl(MAN_LED_RED, OFF);
		break;
	case 4 :
		man_led_ctl(MAN_LED_BUZZER, ON);
		break;
	case 5 :
		man_led_ctl(MAN_LED_BUZZER, OFF);
		break;
	default :
		break;
	}
}

static MbRegisterInstance_t g_regInstanceTab[] = {
	{
        .property = R,
        .address = 0x0000,
		.ReadCb = read_channel_1_full_state,
    },// 读称重1 01 03 00 00 00 01
	{
        .property = R,
        .address = 0x0001,
		.ReadCb = read_channel_2_full_state,
    },// 读称重1 01 03 00 01 00 01
	{
        .property = R,
        .address = 0x0002,
		.ReadCb = read_channel_3_full_state,
    },// 读称重2 01 03 00 02 00 01
	{
        .property = R,
        .address = 0x0003,
		.ReadCb = read_channel_4_full_state,
    },// 读称重2 01 03 00 03 00 01
	{
        .property = R,
        .address = 0x0004,
		.ReadCb = read_channel_5_full_state,
    },// 读称重3 01 03 00 04 00 01
	{
        .property = R,
        .address = 0x0005,
		.ReadCb = read_channel_6_full_state,
    },// 读称重3 01 03 00 05 00 01
	{
        .property = R,
        .address = 0x0006,
		.ReadCb = read_weight_ch4_l,
    },// 读称重4 01 03 00 00 00 01
	{
        .property = R,
        .address = 0x0007,
		.ReadCb = read_weight_ch4_h,
    },// 读称重4 01 03 00 01 00 01
	{
        .property = R,
        .address = 0x0008,
		.ReadCb = read_weight_ch5_l,
    },// 读称重5 01 03 00 02 00 01
	{
        .property = R,
        .address = 0x0009,
		.ReadCb = read_weight_ch5_h,
    },// 读称重5 01 03 00 03 00 01
	{
        .property = R,
        .address = 0x000a,
		.ReadCb = read_weight_ch6_l,
    },// 读称重6 01 03 00 04 00 01
	{
        .property = R,
        .address = 0x000b,
		.ReadCb = read_weight_ch6_h,
    },// 读称重6 01 03 00 05 00 01
	{
        .property = R,
        .address = 0x000c,
		.ReadCb = read_full_state_l,
    },// 读满桶状态 01 03 00 06 00 01
	{
        .property = R,
        .address = 0x000d,
		.ReadCb = read_full_state_h,
    },// 读满桶状态 01 03 00 06 00 01
	{
        .property = R,
        .address = 0x000e,
		.ReadCb = read_bags_broken_state,
    },// 读破袋工作状态 01 03 00 07 00 01
	{
        .property = R,
        .address = 0x000f,
		.ReadCb = read_door_open_state,
    },// 读投口打开状态 01 03 00 08 00 01
	{
        .property = R,
        .address = 0x0010,
		.ReadCb = read_door_close_state,
    },// 读投口关闭状态 01 03 00 09 00 01
	{
        .property = R,
        .address = 0x0011,
		.ReadCb = read_autochange_work_state,
    },// 读换桶工作状态 01 03 00 0a 00 01
	{
        .property = R,
        .address = 0x0012,
		.ReadCb = read_bags_broken_belongs_to,
    },// 查询破袋被注册的通道号 01 03 00 00 00 01
	
	
	{
        .property = W,
        .address = 0x00f1,
		.minValue = 0,
		.maxValue = 6,
		.WriteCb = write_channel_select,
    },// 投口选择          01 06 00 f1 00 00   --取消
      //                   01 06 00 f1 00 01   --第一个投口
	  //                   01 06 00 f1 00 02   --第二个投口
	  //                   01 06 00 f1 00 03   --第三个投口
	{
        .property = W,
        .address = 0x00f2,
		.minValue = 0,
		.maxValue = 0x0c,
		.WriteCb = write_channel_work,
    },// 垃圾桶选择        01 06 00 f2 00 00   --第一个垃圾桶
      //                   01 06 00 f2 00 01   --第二个垃圾桶
	  //                   01 06 00 f2 00 02   --第三个垃圾桶
	  //                   01 06 00 f2 00 03   --自动选择
	  //                   01 06 00 f2 00 04   --推杆打开
	  //                   01 06 00 f2 00 05   --推杆关闭
	  //                   01 06 00 f2 00 06   --推杆停止
      //                   01 06 00 f2 00 07   --光栅失能
	  //                   01 06 00 f2 00 08   --光栅使能
	{
        .property = W,
        .address = 0x00f4,
		.minValue = 0,
		.maxValue = 6,
		.WriteCb = write_autochange_zero,
	},// 投口回零           01 06 00 f4 00 01   --回零
	{
        .property = W,
        .address = 0x00f5,
		.minValue = 0,
		.maxValue = 6,
		.WriteCb = write_full_detect,
	},// 满桶检测           01 06 00 f5 00 00   --通道一满桶检测
	  // 满桶检测           01 06 00 f5 00 01   --通道二满桶检测
	  // 满桶检测           01 06 00 f5 00 02   --通道三满桶检测
	  // 满桶检测           01 06 00 f5 00 03   --所有通道满桶检测
	{
		.property = W,
		.address = 0x00f6,
		.minValue = 0,
		.maxValue = 5,
		.WriteCb = write_man_led_ctl,
	},
	
};
#define REG_TABLE_SIZE         (sizeof(g_regInstanceTab) / sizeof(g_regInstanceTab[0]))


static eMBErrorCode ReadRegsCb(uint8_t startAddr, uint8_t regNum, uint8_t *buf)
{
	if (buf == NULL)
	{
        return MB_EINVAL;
	}
	
    for (uint32_t i = 0; i < regNum; i++)
	{
		MbRegisterInstance_t *instance = NULL;
		for (uint32_t j = 0; j < REG_TABLE_SIZE; j++)
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
	
    for (uint32_t i = 0; i < regNum; i++)
	{
		
		MbRegisterInstance_t *instance = NULL;
		for (uint32_t j = 0; j < REG_TABLE_SIZE; j++)
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
        .baudRate = 9600,
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
