#ifndef _SYSTEMP_H
#define _SYSTEMP_H

#include "stm32h7xx_hal.h"

void SysTemp_Start_Conv(void);
float SysTemp_GET(void);
float VBAT_GET(void);
uint8_t VBAT_GET_PCT(void);

#endif
