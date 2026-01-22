#include "drv_ws2812b.h"
#include "delay.h"
#define WS2812B_HIGH         0
#define WS2812B_LOW          1
#define WS2812B_NUM          16

static void ws2812b_delay(uint32_t n)
{
	while(n --);
}

static __inline void ws2812b_data_out(ws2812b_t *pdev, uint8_t s)
{
	(s == WS2812B_LOW) ? (gpio_bit_set(pdev->gpio_port, pdev->gpio_pin)) :
							(gpio_bit_reset(pdev->gpio_port, pdev->gpio_pin));
}

static void ws2812b_reset(ws2812b_t *pdev)
{
    ws2812b_data_out(pdev, WS2812B_LOW);
    ws2812b_delay(10);
}

static void ws2812b_send_one(ws2812b_t *pdev)
{
	GPIO_BC(pdev->gpio_port) = (uint32_t)pdev->gpio_pin;
	//ws2812b_delay(14);
	ws2812b_delay(14);
	
	GPIO_BOP(pdev->gpio_port) = (uint32_t)pdev->gpio_pin;
	
	//ws2812b_delay(4);
}

static void ws2812b_send_zero(ws2812b_t *pdev)
{
	GPIO_BC(pdev->gpio_port) = (uint32_t)pdev->gpio_pin;
	//ws2812b_delay(3);
	ws2812b_delay(8);
	
	GPIO_BOP(pdev->gpio_port) = (uint32_t)pdev->gpio_pin;
	//ws2812b_delay(12);
	ws2812b_delay(3);
}

void ws2812b_ctl(ws2812b_t *pdev, uint8_t green, uint8_t red, uint8_t blue)
{
	ws2812b_reset(pdev);
	
	uint8_t *tmp_data[] = {&green, &red, &blue};
	
	for(int i = 0; i < WS2812B_NUM; i += 1)
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

