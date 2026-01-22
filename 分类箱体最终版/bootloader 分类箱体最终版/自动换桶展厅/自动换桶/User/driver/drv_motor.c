#include "drv_motor.h"
#include "device.h"
#include "driver.h"

#include "delay.h"

static void motor_stop(device_t *pdev)
{
	gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[1].pin);
	
	pdev->state = MOTOR_STOP;
}

static void motor_on(device_t *pdev)
{
	motor_stop(pdev);
	//delay_nms(100);
	gpio_bit_set(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_bit_reset(pdev->hw_info[1].port, pdev->hw_info[1].pin);
	
	pdev->state = MOTOR_OPEN;
}

static void motor_off(device_t *pdev)
{
	motor_stop(pdev);
	
	gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_bit_set(pdev->hw_info[1].port, pdev->hw_info[1].pin);
	
	pdev->state = MOTOR_CLOSE;
}

static int motor_read(device_t *pdev, void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	*(state_enum_t*)buf = pdev->state;
	
	return count;
}

/* Ð´1¿ª£¬Ð´2¹Ø£¬Ð´0Í£Ö¹ */
static int motor_write(device_t *pdev, const void *buf, size_t count)
{
	if(!pdev)
		return -1;
	
	if(*(int *)buf == 1)
		motor_on(pdev);
	else if(*(int *)buf == 2)
		motor_off(pdev);
	else if(*(int *)buf == 0)
		motor_stop(pdev);
	
	return count;
}

static int motor_ioct(device_t *pdev, uint32_t cmd, void *arg)
{
	if(!pdev)
		return -1;
	
	switch(cmd)
	{
	case CMD_MOTOR_ON :
		motor_on(pdev);
		break;
	case CMD_MOTOR_OFF :
		motor_off(pdev);
		break;
	case CMD_MOTOR_STOP :
		motor_stop(pdev);
		break;
	default :
		break;
	}
	
	return 0;
}

static int motor_close(device_t *pdev)
{
	return 0;
}

static operations_t motor_operations = {
	.read = motor_read,
	.write = motor_write,
	.ioctl = motor_ioct,
	.close = motor_close,
};

static void motor_init(device_t *pdev)
{
	rcu_periph_clock_enable(pdev->hw_info[0].port_rcu);
	rcu_periph_clock_enable(pdev->hw_info[1].port_rcu);
	
	gpio_bit_reset(pdev->hw_info[0].port, pdev->hw_info[0].pin);
	gpio_bit_reset(pdev->hw_info[1].port, pdev->hw_info[1].pin);
	
	gpio_init(pdev->hw_info[0].port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ ,pdev->hw_info[0].pin);
	gpio_init(pdev->hw_info[1].port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ ,pdev->hw_info[1].pin);
}

static int motor_open(device_t *pdev)
{
	if(!pdev)
		return -1;
	
	pdev->ops = &motor_operations;
	
	motor_init(pdev);
	
	pdev->state = MOTOR_STOP;
	
	return 0;
}

static driver_t motor_driver = {
	.name = "motor",
	.open = motor_open,
};

void motor_driver_init(void)
{
	driver_register(&motor_driver);
}
INIT_DRIVE_EXPORT(motor_driver_init)
