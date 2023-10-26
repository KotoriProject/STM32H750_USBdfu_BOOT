#ifndef _VOFA_H
#define _VOFA_H

#include "stm32h7xx_hal.h"

#define CH_COUNT 1 // Vofa+通道数量
#define FRAME_POINT 521


typedef float CHdata;

struct JustFloat
{
    CHdata fdata[CH_COUNT];
    unsigned char tail[4];
};
extern struct JustFloat FRAME[FRAME_POINT];

void Vofa_Fdata_Init();
void Vofa_CH_SEND();
void Vofa_CH_SEND_Cplt();
#endif