#ifndef _RTCSD3078_H
#define _RTCSD3078_H

#include "stm32h7xx_hal.h"

typedef enum
{
    SD3078_ERR,
    SD3078_OK,
} SD3078_STATUS;

typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hou;
} RTC_SD3078;

typedef enum
{
    Write_Protect_ENABLE,
    Write_Protect_DISABLE,
} SD3078_WP_STATUS;

extern RTC_SD3078 SD3078_Time;

SD3078_WP_STATUS SD3078_WriteProtect_GETSTATUS();
uint8_t SD3078_Read_Time(RTC_SD3078 *buf);
uint8_t SD3078_Write_Time(RTC_SD3078 *buf);
#endif