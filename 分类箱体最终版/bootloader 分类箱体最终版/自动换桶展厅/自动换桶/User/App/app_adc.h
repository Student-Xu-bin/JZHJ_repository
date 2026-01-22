#ifndef __APP_ADC_H__
#define __APP_ADC_H__

#include "gd32f30x.h"

void adc_config(void);
uint16_t adc_get_value(uint8_t ch);

#endif /* __APP_ADC_H__ */
