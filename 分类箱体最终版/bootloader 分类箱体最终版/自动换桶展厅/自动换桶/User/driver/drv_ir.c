#include "drv_ir.h"
#include "device.h"
#include "driver.h"

state_enum_t ir_get_state(device_t *pdev)
{
	state_enum_t ret = IR_EMPTY;
	
	if(gpio_input_bit_get(pdev->hw_info[0].port, pdev->hw_info[0].pin) == SET)
		ret = IR_EMPTY;
	else
		ret = IR_FULL;
	
	pdev->state = ret;
	
	return ret;
}

static int ir_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	*(state_enum_t*)buf = ir_get_state(pdev);
	
	return count;
}

static operations_t ir_operations = {
	.read = ir_read,
};

static void ir_init(device_t *pdev)
{
	rcu_periph_clock_enable(pdev->hw_info[0].port_rcu);
	
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ ,pdev->hw_info[0].pin);
}

static int ir_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &ir_operations;
	
	ir_init(pdev);
	
	return 0;
}

static driver_t ir_driver = {
	.name = "ir",
	.open = ir_open,
};

void ir_driver_init(void)
{
	driver_register(&ir_driver);
}
INIT_DRIVE_EXPORT(ir_driver_init)
