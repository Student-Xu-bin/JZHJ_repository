#include "dev_key.h"
void dev_key_gpio_init(void)
{
    rcu_periph_clock_enable(KEY_1_GPIO_RCU_PORT);
    rcu_periph_clock_enable(KEY_5_GPIO_RCU_PORT);

    gpio_bit_set(KEY_1_GPIO_PORT, KEY_1_GPIO_PIN);
    gpio_bit_set(KEY_5_GPIO_PORT, KEY_5_GPIO_PIN);

    gpio_init(KEY_1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY_1_GPIO_PIN);
    gpio_init(KEY_5_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY_5_GPIO_PIN);
}
