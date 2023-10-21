#define GT9XX 1
#if GT9XX

#ifdef _TOUCH_H
#error "GT9XX can not use with GT9XX!"
#else
#define _TOUCH_H

#include "stm32h7xx_hal.h"

#define IIC_ADD (0x5D)
#define IIC_ADD_WRITE (IIC_ADD << 1)
#define IIC_ADD_READ ((IIC_ADD << 1) | 1)
#define GT9XX_POINTS_ADDVALUE (8U)
#define GT9XX_STATUS_ADD (0x814E)
#define GT9XX_POINT1 (0x814F)
#define GT9XX_POINT2 (GT9XX_POINT1 + GT9XX_POINTS_ADDVALUE)
#define GT9XX_POINT3 (GT9XX_POINT2 + GT9XX_POINTS_ADDVALUE)
#define GT9XX_POINT4 (GT9XX_POINT3 + GT9XX_POINTS_ADDVALUE)
#define GT9XX_POINT5 (GT9XX_POINT4 + GT9XX_POINTS_ADDVALUE)
#define GT9XX_MAX_POINT 5
typedef enum
{
    GT9XX_none,
    GT9XX_touch,
} GT9XX_buffer_status;

typedef struct
{
    GT9XX_buffer_status status;
    uint8_t numberOfTouchPoints;
}GT9XX_STATUS;

typedef struct
{
    uint16_t id;
    uint16_t size;
    uint16_t x;
    uint16_t y;
} touchPoint;

extern touchPoint touch_points[GT9XX_MAX_POINT];
extern GT9XX_STATUS touch_status;

void Touch_INT_Callback(void);
void Touch_I2C_READTouch(touchPoint *point);
void TOUCH_INIT(void);

#endif
#endif
