#ifndef _STC_H
#define _STC_H

#include "stm32h7xx_hal.h"

#define DMX_CHANNEL 256
extern uint8_t dmx_buf[DMX_CHANNEL + 1];

#define UART4_MAX_BUF 100
extern volatile uint8_t uart4_buf[UART4_MAX_BUF];
#define UART7_MAX_BUF 100
extern volatile uint8_t uart7_buf[UART7_MAX_BUF];

extern volatile uint16_t SysTemp_buf[20];
extern volatile uint16_t VBAT_buf[20];
#endif
