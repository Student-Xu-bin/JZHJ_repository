#include "dev_user_led.h"

void user_led_gpio_init(void)
{ 
    rcu_periph_clock_enable(USER_LED_RCU_PORT);
    gpio_bit_reset(USER_LED_PORT, USER_LED_PIN);
    gpio_init(USER_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, USER_LED_PIN);
}
