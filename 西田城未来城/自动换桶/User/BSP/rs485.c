#include "rs485.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t modbus_taskhandle;

#define receive_mode(U)    ( (GPIO_BC(U->hw_info.uart_ctl_gpio)) = (uint32_t)(U->hw_info.uart_ctl_pin) )
#define send_mode(U)    ( (GPIO_BOP(U->hw_info.uart_ctl_gpio)) = (uint32_t)(U->hw_info.uart_ctl_pin) )

rs485_t slave_rs485 = {
	.hw_info.uart_ctl_gpio       =   A_UART_CTL_PORT,
    .hw_info.uart_ctl_gpio_rcu   =   A_UART_CTL_PORT_RCU,
    .hw_info.uart_ctl_pin        =   A_UART_CTL_PIN,
    .hw_info.uart_rx_pin         =   A_UART_RX_PIN,
    .hw_info.uart_rx_gpio_rcu    =   A_UART_PORT_RCU,
    .hw_info.uart_tx_gpio_rcu    =   A_UART_PORT_RCU,
    .hw_info.uart_rx_gpio        =   A_UART_PORT,
    .hw_info.uart_tx_pin         =   A_UART_TX_PIN,
    .hw_info.uart_tx_gpio        =   A_UART_PORT,
    .hw_info.uart_no             =   A_UART,
    .hw_info.uart_rcu            =   A_UART_RCU,
    .hw_info.dma_ch              =   A_UART_DMA_CH,
    .hw_info.dma_no              =   A_UART_DMA,
    .hw_info.rcu_dma             =   A_UART_DMA_RCU,
    .hw_info.irq                 =   A_UART_IRQn,
};

static void rs485_gpio_init(rs485_t *u)
{
    rcu_periph_clock_enable(u->hw_info.uart_rx_gpio_rcu);
    if(u->hw_info.uart_rx_gpio != u->hw_info.uart_tx_gpio)
        rcu_periph_clock_enable(u->hw_info.uart_tx_gpio_rcu);

    rcu_periph_clock_enable(u->hw_info.uart_ctl_gpio_rcu);

    gpio_init(u->hw_info.uart_rx_gpio, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, u->hw_info.uart_rx_pin);
    gpio_init(u->hw_info.uart_rx_gpio, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, u->hw_info.uart_tx_pin);

    receive_mode(u);
    gpio_init(u->hw_info.uart_ctl_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, u->hw_info.uart_ctl_pin);
}

static void rs485_dma_init(rs485_t *const u, uint32_t addr)
{
    rs485_hw_info_t const * const temp = &u->hw_info;

    rcu_periph_clock_enable(temp->rcu_dma);

    dma_deinit(temp->dma_no, temp->dma_ch);

    dma_parameter_struct dma_struct = {
        .direction = DMA_PERIPHERAL_TO_MEMORY,
        .periph_addr = addr,
        .periph_inc = DMA_PERIPH_INCREASE_DISABLE,
        .periph_width = DMA_PERIPHERAL_WIDTH_8BIT,
        .memory_addr = (uint32_t)slave_rs485.buffer,
        .memory_inc = DMA_MEMORY_INCREASE_ENABLE,
        .memory_width = DMA_MEMORY_WIDTH_8BIT,
        .number = sizeof(slave_rs485.buffer) / sizeof(uint8_t),
        .priority = DMA_PRIORITY_HIGH
    };

    dma_init(temp->dma_no, temp->dma_ch, &dma_struct);

    usart_dma_receive_config(temp->uart_no, USART_RECEIVE_DMA_ENABLE);

    dma_channel_enable(temp->dma_no, temp->dma_ch);
}

int _rs485_init(rs485_t *u, uint32_t baudrate)
{
	if(!u)
		return -1;
	
	//rs485_gpio_init(u);
	
	rcu_periph_clock_enable(u->hw_info.uart_rcu);

    usart_deinit(u->hw_info.uart_no);
    usart_baudrate_set(u->hw_info.uart_no, baudrate);
    usart_word_length_set(u->hw_info.uart_no, USART_WL_8BIT);
    usart_stop_bit_set(u->hw_info.uart_no, USART_STB_1BIT);
    usart_parity_config(u->hw_info.uart_no, USART_PM_NONE);
    usart_hardware_flow_cts_config(u->hw_info.uart_no, USART_CTS_DISABLE);
    usart_hardware_flow_rts_config(u->hw_info.uart_no, USART_RTS_DISABLE);
    usart_receive_config(u->hw_info.uart_no, USART_RECEIVE_ENABLE);
    usart_transmit_config(u->hw_info.uart_no, USART_TRANSMIT_ENABLE);
	
	usart_interrupt_enable(u->hw_info.uart_no, USART_INT_IDLE);
    nvic_irq_enable(u->hw_info.irq, 6, 0);

    usart_enable(u->hw_info.uart_no);
	
	return 0;
}

static void ttl_gpio_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);

	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
}

static void ttl_init(uint32_t baudRate)
{
	/* 使能UART时钟；*/
	rcu_periph_clock_enable(RCU_USART1);
	/* 复位UART；*/
	usart_deinit (USART1);
	usart_baudrate_set(USART1, baudRate);
	/* 在USART_CTL0寄存器中设置TEN位，使能发送功能；*/
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	/* 在USART_CTL0寄存器中设置TEN位，使能接收功能；*/
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);

	/* 使能串口中断；*/
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	nvic_irq_enable(USART1_IRQn, 6, 0);
	/* 在USART_CTL0寄存器中置位UEN位，使能UART；*/ 
	usart_enable(USART1);
}

void rs485_init(void)
{
	rs485_gpio_init(&slave_rs485);
	_rs485_init(&slave_rs485, A_BAUDRATE);
	rs485_dma_init(&slave_rs485, A_DATA_ADDR);
	
	ttl_gpio_init();
	ttl_init(9600U);//motify
}

int rs485_write(rs485_t *const u, const uint8_t * const buffer, uint8_t size)
{
    uint8_t ret = 0;

    send_mode(u);

    for(ret = 0; ret < size; ret ++)
    {
        usart_interrupt_flag_clear(u->hw_info.uart_no, USART_INT_FLAG_TC);
        USART_DATA(u->hw_info.uart_no) = USART_DATA_DATA & (uint32_t)buffer[ret];
		while(RESET == usart_flag_get(u->hw_info.uart_no, USART_FLAG_TC));
    }

    receive_mode(u);
	
    return ret;
}

void USART0_IRQHandler(void)
{
	BaseType_t pd = pdFALSE;
	
	if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE))
    {
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
		uint8_t ch = usart_data_receive(USART0);
		
		slave_rs485.size = sizeof(slave_rs485.buffer) - dma_transfer_number_get(slave_rs485.hw_info.dma_no, slave_rs485.hw_info.dma_ch);
		
		uint32_t weight_value = 0;
		weight_value = (slave_rs485.buffer[3] << 24) | (slave_rs485.buffer[4] << 16) | (slave_rs485.buffer[5] << 8) | slave_rs485.buffer[6];
		
		xTaskNotifyFromISR(modbus_taskhandle, weight_value, eSetValueWithOverwrite, &pd);
		
        dma_channel_disable(slave_rs485.hw_info.dma_no, slave_rs485.hw_info.dma_ch);
		dma_transfer_number_config(slave_rs485.hw_info.dma_no, slave_rs485.hw_info.dma_ch, sizeof(slave_rs485.buffer));
		dma_channel_enable(slave_rs485.hw_info.dma_no, slave_rs485.hw_info.dma_ch);
    }
}

void USART1_IRQHandler(void)
{
	if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
    {
		usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
		uint8_t ch = usart_data_receive(USART1);
	}
}
