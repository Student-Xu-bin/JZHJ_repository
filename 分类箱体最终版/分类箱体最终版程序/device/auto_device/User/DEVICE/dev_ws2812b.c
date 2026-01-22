#include "dev_ws2812b.h"

ws2812b_t ws2812b[6] = {
    [0] = {
        .rcu_port   = WS2812B_1_RCU_PORT,
        .gpio_port  = WS2812B_1_PORT,
        .gpio_pin   = WS2812B_1_PIN,
    },
    [1] = {
        .rcu_port   = WS2812B_2_RCU_PORT,
        .gpio_port  = WS2812B_2_PORT,
        .gpio_pin   = WS2812B_2_PIN,
    },
    [2] = {
        .rcu_port   = WS2812B_3_RCU_PORT,
        .gpio_port  = WS2812B_3_PORT,
        .gpio_pin   = WS2812B_3_PIN,
    },
    [3] = {
        .rcu_port   = WS2812B_4_RCU_PORT,
        .gpio_port  = WS2812B_4_PORT,
        .gpio_pin   = WS2812B_4_PIN,
    },
    [4] = {
        .rcu_port   = WS2812B_5_RCU_PORT,
        .gpio_port  = WS2812B_5_PORT,
        .gpio_pin   = WS2812B_5_PIN,
    },
    [5] = {
        .rcu_port   = WS2812B_6_RCU_PORT,
        .gpio_port  = WS2812B_6_PORT,
        .gpio_pin   = WS2812B_6_PIN,
    }
};
void dev_ws2812b_gpio_init(ws2812b_t *p)
{
    rcu_periph_clock_enable(p->rcu_port);
    gpio_bit_set(p->gpio_port, p->gpio_pin);
    gpio_init(p->gpio_port, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX, p->gpio_pin);
}
