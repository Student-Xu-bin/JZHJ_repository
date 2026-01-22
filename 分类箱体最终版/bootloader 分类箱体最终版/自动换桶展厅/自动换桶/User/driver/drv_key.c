#include "drv_key.h"
#include "device.h"
#include "driver.h"

state_enum_t key_get_state(device_t *pdev)
{
	state_enum_t ret = KEY_LOOSE;
	
	if(gpio_input_bit_get(pdev->hw_info[0].port, pdev->hw_info[0].pin) == SET)
		ret = KEY_LOOSE;
	else
		ret = KEY_PRESSED;
	
	pdev->state = ret;
	
	return ret;
}

static int key_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	*(state_enum_t*)buf = key_get_state(pdev);
	
	return count;
}

static operations_t key_operations = {
	.read = key_read,
};

static void key_init(device_t *pdev)
{
	rcu_periph_clock_enable(pdev->hw_info[0].port_rcu);
	
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ ,pdev->hw_info[0].pin);
}

static int key_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &key_operations;
	
	key_init(pdev);
	
	return 0;
}

static driver_t key_driver = {
	.name = "key",
	.open = key_open,
};

void key_driver_init(void)
{
	driver_register(&key_driver);
}
INIT_DRIVE_EXPORT(key_driver_init)
