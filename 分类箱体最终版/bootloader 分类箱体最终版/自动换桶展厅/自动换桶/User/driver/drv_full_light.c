#include "drv_full_light.h"
#include "device.h"
#include "driver.h"

#if (JZHJ_FULL_LIGHT == WS2812B)
#define FULL_LIGHT_LOW 		1
#define FULL_LIGHT_HIGH 	0

static int full_light_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	*(state_enum_t*)buf = pdev->state;
	
	return count;
}

static void full_light_delay(uint32_t n)
{
	while(n --);
}

static __inline void ws2812b_data_out(device_t *pdev, uint8_t s)
{
	(s == FULL_LIGHT_LOW) ? (gpio_bit_set(pdev->hw_info->port, pdev->hw_info->pin)) :
							(gpio_bit_reset(pdev->hw_info->port, pdev->hw_info->pin));
}

static void ws2812b_reset(device_t *pdev)
{
    ws2812b_data_out(pdev, FULL_LIGHT_LOW);
    full_light_delay(10);
}

static void ws2812b_send_one(device_t *pdev)
{
	ws2812b_data_out(pdev, FULL_LIGHT_HIGH);
	full_light_delay(12);
	
	ws2812b_data_out(pdev, FULL_LIGHT_LOW);
	full_light_delay(10);
}

static void ws2812b_send_zero(device_t *pdev)
{
	ws2812b_data_out(pdev, FULL_LIGHT_HIGH);
	full_light_delay(3);
	
	ws2812b_data_out(pdev, FULL_LIGHT_LOW);
	full_light_delay(8);
}

static void full_light_ctl(device_t *pdev, uint8_t green, uint8_t red, uint8_t blue)
{
	ws2812b_reset(pdev);
	
	uint8_t *tmp_data[] = {&green, &red, &blue};
	
	for(int i = 0; i < pdev->hw_info->private_data; i += 1)
	{
		for(int j = 0; j < (sizeof(tmp_data) << 1); j += 1)
		{
			if(*tmp_data[j / 8] & (0x80 >> (j % 8)))
				ws2812b_send_one(pdev);
			else
				ws2812b_send_zero(pdev);
		}
	}
}
#endif

#if (JZHJ_FULL_LIGHT == WS2812B)
/* RGB */
static int full_light_write(device_t *pdev, const void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	uint8_t tmp_data[3] = {0};
	
	if(count > 3)
		count = 3;
	
	for(int i = 0; i < count; i ++)
	{
		tmp_data[i] = ((uint8_t *)buf)[i];
	}
	
	if((tmp_data[0] == 0) && (tmp_data[1] == 0) && (tmp_data[2] != 0))
		pdev->state = LIGHT_BLUE;
	else if((tmp_data[0] == 0) && (tmp_data[1] != 0) && (tmp_data[2] == 0))
		pdev->state = LIGHT_GREEN;
	else if((tmp_data[0] != 0) && (tmp_data[1] == 0) && (tmp_data[2] == 0))
		pdev->state = LIGHT_RED;
	else if((tmp_data[0] != 0) && (tmp_data[1] != 0) && (tmp_data[2] == 0))
		pdev->state = LIGHT_YELLOW;
	else
		pdev->state = LIGHT_OTHER;
	
	full_light_ctl(pdev, tmp_data[1], tmp_data[0], tmp_data[2]);
	
	return count;
}
#elif (JZHJ_FULL_LIGHT == RED_LIGHT)
static int full_light_write(device_t *pdev, const void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	if((*(char *)buf) == 1)
	{
		pdev->state = ON;
		gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	}
	else
	{
		pdev->state = OFF;
		gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	}
	
	return count;
}
#endif


#if (JZHJ_FULL_LIGHT == WS2812B)
static int full_light_ioct(device_t *pdev, uint32_t cmd, void *arg)
{
	if(!pdev)
		return -1;
	
	switch(cmd)
	{
	case CMD_FULL_LIGHT_GREEN :
		pdev->state = LIGHT_GREEN;
		full_light_ctl(pdev, 0xff, 0, 0);
		break;
	case CMD_FULL_LIGHT_RED :
		pdev->state = LIGHT_RED;
		full_light_ctl(pdev, 0, 0xff, 0);
		break;
	case CMD_FULL_LIGHT_BLUE :
		pdev->state = LIGHT_BLUE;
		full_light_ctl(pdev, 0, 0, 0xff);
		break;
	case CMD_FULL_LIGHT_YELLOW :
		pdev->state = LIGHT_YELLOW;
		full_light_ctl(pdev, 0xff, 0xff, 0);
		break;
	default :
		break;
	}
	
	return 0;
}
#elif (JZHJ_FULL_LIGHT == RED_LIGHT)
static int full_light_ioct(device_t *pdev, uint32_t cmd, void *arg)
{
	if(!pdev)
		return -1;
	
	switch(cmd)
	{
	case CMD_FULL_LIGHT_ON :
		pdev->state = ON;
		gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);
		break;
	case CMD_FULL_LIGHT_OFF :
		pdev->state = OFF;
		gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
		break;
	default :
		break;
	}
	
	return 0;
}
#endif
static int full_light_close(device_t *pdev)
{
	return 0;
}

static operations_t full_light_operations = {
	.read = full_light_read,
	.write = full_light_write,
	.ioctl = full_light_ioct,
	.close = full_light_close,
};

static void full_light_init(device_t *pdev)
{
	rcu_periph_clock_enable(pdev->hw_info[0].port_rcu);

#if (JZHJ_FULL_LIGHT == WS2812B)
	gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ ,pdev->hw_info[0].pin);
#elif (JZHJ_FULL_LIGHT == RED_LIGHT)
	gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ ,pdev->hw_info[0].pin);
#endif
}

static int full_light_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &full_light_operations;
	
	full_light_init(pdev);
	
	return 0;
}

static driver_t full_light_driver = {
	.name = "full_light",
	.open = full_light_open,
};

void full_light_driver_init(void)
{
	driver_register(&full_light_driver);
}
INIT_DRIVE_EXPORT(full_light_driver_init)
