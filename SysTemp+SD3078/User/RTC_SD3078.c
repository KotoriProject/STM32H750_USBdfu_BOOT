#include "RTC_SD3078.h"

#include "Soft_iic.h"
#include "main.h"

RTC_SD3078 SD3078_Time = {0};

SOFT_IIC_HandleTypeDef SD3078_IIC = {
    .IIC_7bitADDR = 0x32,
    .SCL_Port = RTC_SCL_GPIO_Port,
    .SCL_Pin = RTC_SCL_Pin,
    .SDA_Port = RTC_SDA_GPIO_Port,
    .SDA_Pin = RTC_SDA_Pin};

#define BCDtoDEC(bcd) ((int)(bcd) / 16 * 10 + (int)(bcd) % 16)
#define DECtoBCD(dec) ((int)(dec) / 10 * 16 + (int)(dec) % 10)

#define SD3078_CTR1 (0x0F)
#define SD3078_CTR2 (0x10)

/*检查是否出现过停振或完全断电*/
void SD3078_IIC_Init()
{
}

/*WRTC1、WRTC2、WRTC3 位: 寄存器（00H~1FH）写允许位。即 WRTC1=1、WRTC2=1、WRTC3=1
时写允许.注意置位有先后顺序，先置 WRTC1 为 1,后置 WRTC2、WRTC3 为 1;当 WRTC1=0、
WRTC2=0、WRTC3=0 时则写禁止，同样置位有先后顺序，先置 WRTC2、WRTC3 为 0,后置 WRTC1 为
0。当写禁止时，除了以上三位可以写以外，从 00H 到 79H 所有的寄存器均不可以写。写禁止并不
影响读操作。
特别的:当写允许时,如需要赋值与写保护位相关的寄存器 0FH、10H,则要注意对应的位 WRTC1、
WRTC2、WRTC3 的赋值,这三个位只能为 1 而不可为 0,否则会造成写禁止而数据写不进相应
的寄存器.又因为 0F 的寄存器其它位均为标志位，所以建议写允许时赋值 0F 寄存器的值可
以定为 FFH，写禁止时赋值 0F 寄存器的值可以定为 7BH。*/
SD3078_WP_STATUS SD3078_WriteProtect_GETSTATUS()
{
    uint8_t WRTC[2] = {0};
    Soft_IIC_ReadREG(&SD3078_IIC, SD3078_CTR1, WRTC, 2);
    if ((WRTC[0] >> 7) & 1 && (WRTC[0] >> 2) & 1 && (WRTC[1] >> 7) & 1)
        return Write_Protect_DISABLE;
    else
        return Write_Protect_ENABLE;
}

uint8_t SD3078_Read_Time(RTC_SD3078 *buf)
{
    uint8_t a[3];
    if (Soft_IIC_ReadREG(&SD3078_IIC, 0x00, a, 3) == 0)
        return SD3078_ERR;
    buf->sec = a[0] & 0x7F;
    buf->min = a[1] & 0x7F;
    if (a[2] >> 7 == 1) // 24H制
    {
        buf->hou = a[2] & 0x1F; // 前5位有效
    }
    else // 12H制
    {
        uint8_t bcd;
        bcd = a[2] & 0x1F;
        if ((a[2] >> 5) & 1) // PM
        {
            bcd = DECtoBCD(BCDtoDEC(bcd) + 12);
            buf->hou = bcd;
        }
        else // AM
        {
            buf->hou = a[2] & 0x1F;
        }
    }
    return SD3078_OK;
}

uint8_t SD3078_Write_Time(RTC_SD3078 *buf)
{
    if (SD3078_WriteProtect_GETSTATUS() == Write_Protect_DISABLE)
    {
        Soft_IIC_WriteREG(&SD3078_IIC, 0x00, (uint8_t *)buf, 3);
    }
    else
        return SD3078_ERR;
}
