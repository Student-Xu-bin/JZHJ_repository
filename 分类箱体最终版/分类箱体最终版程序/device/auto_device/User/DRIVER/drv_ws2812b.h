#ifndef __DRV_WS2812B_H
#define __DRV_WS2812B_H

#include "gd32f30x.h"
#include "dev_ws2812b.h"

void ws2812b_ctl(ws2812b_t *pdev, uint8_t green, uint8_t red, uint8_t blue);

#endif
