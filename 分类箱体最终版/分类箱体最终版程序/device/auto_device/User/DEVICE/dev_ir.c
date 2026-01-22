#include "dev_ir.h"

void dev_ir_gpio_init(void)
{
    rcu_periph_clock_enable(IR_1_GPIO_RCU_PORT);
    rcu_periph_clock_enable(IR_4_GPIO_RCU_PORT);
    rcu_periph_clock_enable(IR_5_GPIO_RCU_PORT);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);

    gpio_bit_set(IR_1_GPIO_PORT, IR_1_GPIO_PIN);
    gpio_bit_set(IR_4_GPIO_PORT, IR_4_GPIO_PIN);
    gpio_bit_set(IR_5_GPIO_PORT, IR_5_GPIO_PIN);

    gpio_init(IR_1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, IR_1_GPIO_PIN);
    gpio_init(IR_4_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, IR_4_GPIO_PIN);
    gpio_init(IR_5_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, IR_5_GPIO_PIN);
}


