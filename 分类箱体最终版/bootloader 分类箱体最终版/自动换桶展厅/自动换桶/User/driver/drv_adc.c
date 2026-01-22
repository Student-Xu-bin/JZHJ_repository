#include "drv_adc.h"
#include "driver.h"
#include "device.h"

static int adc_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	
	
	return count;
}

static int adc_write(device_t *pdev, const void *buf, size_t count)
{
	if(!pdev)
		return -1;

	
	
	return count;
}

static int adc_ioctl(device_t *pdev, uint32_t cmd, void *arg)
{
	if(!pdev)
		return -1;
	
	
	return 0;
}

static operations_t adc_operations = {
	.read = adc_read,
	.write = adc_write,
	.ioctl = adc_ioctl,
};

static void adc_gpio_init(device_t *pdev)
{
	rcu_periph_clock_enable(pdev->hw_info->port_rcu);
	rcu_periph_clock_enable(RCU_AF);
	
	gpio_init(pdev->hw_info->port, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, pdev->hw_info->pin);
}

static void adc_init(device_t *pdev)
{
	adc_gpio_init(pdev);
}

static int adc_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &adc_operations;
	
	adc_init(pdev);
	
	pdev->state = ON;
	
	return 0;
}

static driver_t driver_adc_channel = {
	.name = "adc_channel",
	.open = adc_open,
};

void adc_driver_init(void)
{
	driver_register(&driver_adc_channel);
}
INIT_DRIVE_EXPORT(adc_driver_init)
