#include "Vofa+.h"

#include "usbd_cdc_if.h"

typedef enum
{
    idle,
    tx,
    rx,
    err,
    busy,
} Vofa_STATUS;
struct
{
    Vofa_STATUS STATUS;
} Vofa_struct = {
    .STATUS = idle,
};

struct JustFloat FRAME[FRAME_POINT];

void Vofa_Fdata_Init()
{
    for (uint32_t i = 0; i < FRAME_POINT; i++)
    {
        FRAME[i].fdata[0] = sinf((float)i / FRAME_POINT * 3.1415926 * 2 - 3.1415926);

        FRAME[i].tail[0] = 0x00;
        FRAME[i].tail[1] = 0x00;
        FRAME[i].tail[2] = 0x80;
        FRAME[i].tail[3] = 0x7F;
    }
}

void Vofa_CH_SEND()
{
    // if (Vofa_struct.STATUS == err)
    //     return;
    // uint8_t count = 0;
    // while (Vofa_struct.STATUS != idle)
    // {
    //     if (count++ >= 255)
    //     {
    //         HAL_Delay(1);
    //         Vofa_struct.STATUS = busy;
    //         return;
    //     }
    // }
    // if (Vofa_struct.STATUS == busy)
    //     return;
    // Vofa_struct.STATUS = tx;
    CDC_Transmit_FS(&FRAME, (CH_COUNT * 4 + 4) * FRAME_POINT);
}

void Vofa_CH_SEND_Cplt()
{
    if (Vofa_struct.STATUS == tx || Vofa_struct.STATUS == busy)
        Vofa_struct.STATUS = idle;
}
