#include "drv_led.h"
#include "driver.h"
#include "device.h"

static void led_on(device_t *pdev)
{
	gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);	
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ ,pdev->hw_info[0].pin);
	
	pdev->state = ON;
}

static void led_off(device_t *pdev)
{
	//gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);	
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_IPD, GPIO_OSPEED_2MHZ ,pdev->hw_info[0].pin);
	pdev->state = OFF;
}

static int led_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	*(state_enum_t*)buf = pdev->state;
	
	return count;
}

static int led_write(device_t *pdev, const void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	if(*(int *)buf == 1)
		led_on(pdev);
	else if(*(int *)buf == 0)
		led_off(pdev);
	
	return count;
}

static int led_ioct(device_t *pdev, uint32_t cmd, void *arg)
{
	if(!pdev)
		return -1;
	
	switch(cmd)
	{
	case CMD_BSP_LED_ON :
		led_on(pdev);
		break;
	case CMD_BSP_LED_OFF :
		led_off(pdev);
		break;
	default :
		break;
	}
	
	return 0;
}

static int led_close(device_t *pdev)
{
	return 0;
}

static operations_t led_operations = {
	.read = led_read,
	.write = led_write,
	.ioctl = led_ioct,
	.close = led_close,
};

static void led_init(device_t *pdev)
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(pdev->hw_info[0].port_rcu);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	
	//gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_IPD, GPIO_OSPEED_2MHZ ,pdev->hw_info[0].pin);
}

static int led_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &led_operations;
	
	led_init(pdev);
	
	pdev->state = OFF;
	
	return 0;
}

static driver_t led_driver = {
	.name = "bsp_led",
	.open = led_open,
};

void led_driver_init(void)
{
	driver_register(&led_driver);
}
INIT_DRIVE_EXPORT(led_driver_init)
