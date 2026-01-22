#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include "dev_adc.h"
#include "delay.h"

uint16_t adc_get_value(uint8_t ch);
void adc_init_config(void);

#endif
