#include "app_adc.h"
#include "device_system.h"
#include "FreeRTOS.h"
#include "delay.h"

static uint16_t adc_values[CHANNEL_NUMBERS];

static void adc_dma_init(void);

void adc_config(void)
{
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_ADC0);
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
	
	adc_dma_init();
	
	device_t *adc_channels[CHANNEL_NUMBERS] = {0};
	
	adc_channels[0] = device_open("adc_channel_1");
#if (CHANNEL_NUMBERS == 1)
	goto open_end;
#endif
	adc_channels[1] = device_open("adc_channel_2");
#if (CHANNEL_NUMBERS == 2)
	goto open_end;
#endif
	adc_channels[2] = device_open("adc_channel_3");
#if (CHANNEL_NUMBERS == 3)
	goto open_end;
#endif
	adc_channels[3] = device_open("adc_channel_4");
#if (CHANNEL_NUMBERS == 4)
	goto open_end;
#endif
	adc_channels[4] = device_open("adc_channel_5");
#if (CHANNEL_NUMBERS == 5)
	goto open_end;
#endif
	adc_channels[5] = device_open("adc_channel_6");

#if (CHANNEL_NUMBERS != 6)
open_end :
#endif
	
	adc_deinit(ADC0);
	
	adc_mode_config(ADC_MODE_FREE);
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, CHANNEL_NUMBERS);
	for(int i = 0; i < CHANNEL_NUMBERS; i ++)
		adc_regular_channel_config(ADC0, i, adc_channels[i]->hw_info->private_data, ADC_SAMPLETIME_55POINT5);
	
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	
	adc_enable(ADC0);
	delay_nms(3);
	adc_calibration_enable(ADC0);
	
	adc_dma_mode_enable(ADC0);
	
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
	
	
}

static void adc_dma_init(void)
{
	dma_parameter_struct dma_config = {0};
	
	dma_deinit(DMA0, DMA_CH0);
	
	dma_config.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
	dma_config.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_config.memory_addr  = (uint32_t)adc_values;
	dma_config.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_config.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_config.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_config.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_config.number 		= CHANNEL_NUMBERS;
	dma_config.priority     = DMA_PRIORITY_HIGH;
	
	dma_init(DMA0, DMA_CH0, &dma_config);
	dma_circulation_enable(DMA0, DMA_CH0);
	
	dma_channel_enable(DMA0, DMA_CH0);
}

uint16_t adc_get_value(uint8_t ch)
{
	return adc_values[ch];
}
