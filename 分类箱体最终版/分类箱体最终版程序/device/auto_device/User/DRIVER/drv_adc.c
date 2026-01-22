#include "drv_adc.h"

static uint16_t adc_values[6];

void adc_init_config(void)
{
	dev_adc_gpio_init();
	
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_ADC0);
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
	
	dma_parameter_struct dma_config = {0};
	
	dma_deinit(DMA0, DMA_CH0);
	
	dma_config.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
	dma_config.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_config.memory_addr  = (uint32_t)adc_values;
	dma_config.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_config.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_config.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_config.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_config.number 		= 6;
	dma_config.priority     = DMA_PRIORITY_HIGH;
	
	dma_init(DMA0, DMA_CH0, &dma_config);
	dma_circulation_enable(DMA0, DMA_CH0);
	
	dma_channel_enable(DMA0, DMA_CH0);
	
	adc_deinit(ADC0);
	
	adc_mode_config(ADC_MODE_FREE);
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 6);
	
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_1, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_0, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_14, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_15, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_8, ADC_SAMPLETIME_55POINT5);
	adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_9, ADC_SAMPLETIME_55POINT5);
		
	
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	
	adc_enable(ADC0);
	delay_nms(3);
	adc_calibration_enable(ADC0);
	
	adc_dma_mode_enable(ADC0);
	
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}


uint16_t adc_get_value(uint8_t ch)
{
	return adc_values[ch];
}


