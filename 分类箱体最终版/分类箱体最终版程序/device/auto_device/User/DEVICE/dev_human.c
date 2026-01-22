#include "dev_human.h"

void dev_human_gpio_init(void)
{
    rcu_periph_clock_enable(HUMAN_1_GPIO_RCU_PORT);
    rcu_periph_clock_enable(HUMAN_5_GPIO_RCU_PORT);

    gpio_bit_set(HUMAN_1_GPIO_PORT, HUMAN_1_GPIO_PIN);
    gpio_bit_set(HUMAN_5_GPIO_PORT, HUMAN_5_GPIO_PIN);

    gpio_init(HUMAN_1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, HUMAN_1_GPIO_PIN);
    gpio_init(HUMAN_5_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, HUMAN_5_GPIO_PIN);
}

