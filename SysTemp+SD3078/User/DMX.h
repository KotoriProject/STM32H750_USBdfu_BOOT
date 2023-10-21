#ifndef _DMX_H
#define _DMX_H

#include "stm32h7xx_hal.h"

void DMX_Init(TIM_HandleTypeDef *htim);
void DMX_DATA_CHANGE(uint16_t channel ,uint8_t data);
void DMX_Task(TIM_HandleTypeDef *htim);

#endif
