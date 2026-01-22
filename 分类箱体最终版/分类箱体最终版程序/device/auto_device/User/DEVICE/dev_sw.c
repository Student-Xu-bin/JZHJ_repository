#include "dev_sw.h"

void sw_gpio_init(void)
{
    rcu_periph_clock_enable(SW_HUMAN_RCU_PORT);
    rcu_periph_clock_enable(SW_LIGHT_RCU_PORT);

    gpio_bit_set(SW_HUMAN_PORT, SW_HUMAN_PIN);
    gpio_bit_set(SW_LIGHT_PORT, SW_LIGHT_PIN);

    gpio_init(SW_HUMAN_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SW_HUMAN_PIN);
    gpio_init(SW_LIGHT_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SW_LIGHT_PIN);
}
