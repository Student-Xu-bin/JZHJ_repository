#include "74hc595.h"
#include "string.h"


static _74hc595_t hc595 = {
        .ds_gpio        = HC959_DS_GPIO,
        .ds_pin         = HC595_DS_PIN,
        .ds_gpio_rcu    = HC595_DS_RCU,
        .shcp_gpio      = HC595_SHCP_GPIO,
        .shcp_pin       = HC595_SHCP_PIN,
        .shcp_gpio_rcu  = HC595_SHCP_RCU,
        .stcp_gpio      = HC595_STCP_GPIO,
        .stcp_pin       = HC959_STCP_PIN,
        .stcp_gpio_rcu  = HC595_STCP_RCU,
};

static union _74hc595_data_bit hc595_send_data = {0};

#define DS_OUT(x)   ( ((x) == 0) ? (gpio_bit_reset(hc595.ds_gpio, hc595.ds_pin)) : (gpio_bit_set(hc595.ds_gpio, hc595.ds_pin)) )
#define STCP_OUT(x)   ( ((x) == 0) ? (gpio_bit_reset(hc595.stcp_gpio, hc595.stcp_pin)) : (gpio_bit_set(hc595.stcp_gpio, hc595.stcp_pin)) )
#define SHCP_OUT(x)   ( ((x) == 0) ? (gpio_bit_reset(hc595.shcp_gpio, hc595.shcp_pin)) : (gpio_bit_set(hc595.shcp_gpio, hc595.shcp_pin)) )

static void _74hc595_gpio_init(_74hc595_t *cfg_struct)
{
    rcu_periph_clock_enable(cfg_struct->ds_gpio_rcu);
    rcu_periph_clock_enable(cfg_struct->shcp_gpio_rcu);
    rcu_periph_clock_enable(cfg_struct->stcp_gpio_rcu);

    gpio_bit_reset(cfg_struct->ds_gpio, cfg_struct->ds_pin);
    gpio_bit_reset(cfg_struct->shcp_gpio, cfg_struct->shcp_pin);
    gpio_bit_reset(cfg_struct->stcp_gpio, cfg_struct->stcp_pin);

    gpio_init(cfg_struct->ds_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, cfg_struct->ds_pin);
    gpio_init(cfg_struct->shcp_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, cfg_struct->shcp_pin);
    gpio_init(cfg_struct->stcp_gpio, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, cfg_struct->stcp_pin);
}

static void _74hc595_write_byte(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i += 1)
    {
        DS_OUT(data >> 7);
		data <<= 1;
		
        SHCP_OUT(1);
        SHCP_OUT(0);
    }
}

static __inline void _74hc595_output(void)
{
    STCP_OUT(1);
    STCP_OUT(0);
}

static inline void _74hc595_write_all(union _74hc595_data_bit *data)
{
    for(int i = 0; i < sizeof(data->data); i += 1)
        _74hc595_write_byte(data->data[i]);
}

void _74hc595_init(void)
{
    _74hc595_gpio_init(&hc595);

    memset(&hc595_send_data, 0, sizeof(union _74hc595_data_bit));
    _74hc595_write_all(&hc595_send_data);
    _74hc595_output();
}

#if 0
static void _74hc595_send_data_buffer_clear(union _74hc595_data_bit *data)
{
    memset(data, 0, sizeof(union _74hc595_data_bit));
}
#endif
#if 0
void _74hc595_ctl(union _74hc595_data_bit *data)
{
	_74hc595_write_all(data);
	_74hc595_output();
}
#else
void _74hc595_ctl(enum channel_enum_t ch, enum function_enum_t fun, state_enum_t state, uint8_t set_)
{
	//hc595_send_data.bit.ch1_cd4051_a0 = 0;
	//hc595_send_data.bit.ch1_cd4051_a1 = 0;
	//hc595_send_data.bit.ch1_cd4051_a2 = 0;
	
	switch(ch)
    {
    case CHANNEL_1 :
        if(fun == RED_LED)
            hc595_send_data.bit.en_red_led_1 = (state == ON) ? 1 : 0;
        break;
    case CHANNEL_2 :
        if(fun == RED_LED)
            hc595_send_data.bit.en_red_led_2 = (state == ON) ? 1 : 0;
        break;
    case CHANNEL_3 :
        if(fun == RED_LED)
            hc595_send_data.bit.en_red_led_3 = (state == ON) ? 1 : 0;
        break;
    case CHANNEL_4 :
        if(fun == RED_LED)
            hc595_send_data.bit.en_red_led_4 = (state == ON) ? 1 : 0;
        break;
	case CHANNEL_5 :
        if(fun == RED_LED)
            hc595_send_data.bit.en_red_led_5 = (state == ON) ? 1 : 0;
        break;
    case CHANNEL_6 :
        if(fun == RED_LED)
            hc595_send_data.bit.en_red_led_6 = (state == ON) ? 1 : 0;
        break;
    case RS485_SLAVE :
        if(fun == RS485_RECEIVE)
            hc595_send_data.bit.rs485_slave_ctl = 0;
		else if(fun == RS485_SEND)
			hc595_send_data.bit.rs485_slave_ctl = 1;
		break;
    case RS485_MASTER :
        if(fun == RS485_RECEIVE)
            hc595_send_data.bit.rs485_master_ctl = 0;
		else if(fun == RS485_SEND)
			hc595_send_data.bit.rs485_master_ctl = 1;
		break;
    default :
        break;
    }

    if(!set_)
        return;

    _74hc595_write_all(&hc595_send_data);
	_74hc595_output();
}
#endif
