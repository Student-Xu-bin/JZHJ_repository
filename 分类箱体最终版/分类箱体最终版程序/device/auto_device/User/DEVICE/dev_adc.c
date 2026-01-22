#include "dev_adc.h"

void dev_adc_gpio_init(void)
{
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(ADC_1_RCU_GPIO);
    rcu_periph_clock_enable(ADC_2_RCU_GPIO);
    rcu_periph_clock_enable(ADC_3_RCU_GPIO);
    rcu_periph_clock_enable(ADC_4_RCU_GPIO);
    rcu_periph_clock_enable(ADC_5_RCU_GPIO);
    rcu_periph_clock_enable(ADC_6_RCU_GPIO);

    gpio_bit_reset(ADC_1_GPIO, ADC_1_PIN);
    gpio_bit_reset(ADC_2_GPIO, ADC_2_PIN);
    gpio_bit_reset(ADC_3_GPIO, ADC_3_PIN);
    gpio_bit_reset(ADC_4_GPIO, ADC_4_PIN);
    gpio_bit_reset(ADC_5_GPIO, ADC_5_PIN);
    gpio_bit_reset(ADC_6_GPIO, ADC_6_PIN);

    gpio_init(ADC_1_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_1_PIN);
    gpio_init(ADC_2_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_2_PIN);
    gpio_init(ADC_3_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_3_PIN);
    gpio_init(ADC_4_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_4_PIN);
    gpio_init(ADC_5_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_5_PIN);
    gpio_init(ADC_6_GPIO, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC_6_PIN);
}
