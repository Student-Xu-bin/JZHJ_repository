#include "74hc165.h"

void _74hc165_gpio_init(void)
{
    rcu_periph_clock_enable(HC165_PL_RCU);
    rcu_periph_clock_enable(HC165_DATA_RCU);
    rcu_periph_clock_enable(HC165_CP_RCU);

    gpio_bit_reset(HC165_PL_GPIO, HC165_PL_PIN);
    gpio_bit_reset(HC165_CP_GPIO, HC165_CP_PIN);
    gpio_bit_reset(HC165_DATA_GPIO, HC165_DATA_PIN);

    gpio_init(HC165_PL_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, HC165_PL_PIN);
    gpio_init(HC165_CP_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, HC165_CP_PIN);
    gpio_init(HC165_DATA_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, HC165_DATA_PIN);
}

uint16_t _74hc165_read_data(void)
{
    uint16_t temp = 0;

    gpio_bit_reset(HC165_PL_GPIO, HC165_PL_PIN);//读取数据到165寄存器
    gpio_bit_set(HC165_PL_GPIO, HC165_PL_PIN);
	
	if(gpio_input_bit_get(HC165_DATA_GPIO, HC165_DATA_PIN) == RESET)
			temp |= 0x01;

    for (int i = 1; i < 16; i++)
    {
        gpio_bit_reset(HC165_CP_GPIO, HC165_CP_PIN);//弹出一位数据
        __NOP();
        __NOP();
        gpio_bit_set(HC165_CP_GPIO, HC165_CP_PIN);

		if(gpio_input_bit_get(HC165_DATA_GPIO, HC165_DATA_PIN) == RESET)
			temp |= (1 << i);
    }

    return temp;
}
