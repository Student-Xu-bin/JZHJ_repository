#ifndef __APP_H__
#define __APP_H__

#include "channel.h"
#include "mankey.h"

void stepper_find_zero(channel_t *channel);
void channel_full_detect(channel_t *channel);
void full_light_show(channel_t *channel);
void throw_garbage(channel_t *channel, uint32_t which);
void gs_detect(channel_t *);
void key_detect(channel_t *);
void proximity_switch_detect(channel_t *channel);
void door_key_led(channel_t *channel);
void stepper_alm_detect(channel_t *channel);
extern FlagStatus zero_dir_flag;
#endif
