#ifndef __GARBAGE_ADC_H
#define __GARBAGE_ADC_H

#include "gd32f30x.h"
#include "delay.h"

class garbage_adc
{
	public:
		garbage_adc();
		uint16_t adc_values[6];
};

#endif
