#define FT5436 0
#if FT5436


#ifdef _TOUCH_H
#error "GT9XX can not use with FT5436!"
#else

#include "stm32h7xx_hal.h"

#define IIC_ADD (0x38)
#define IIC_ADD_WRITE (IIC_ADD << 1)
#define IIC_ADD_READ ((IIC_ADD << 1) | 1)
#define FT5436_STATUS_ADD (0x02)
#define FT5436_POINT1 (0x03)
#define FT5436_POINT2 (FT5436_POINT1 + 6U)
#define FT5436_POINT3 (FT5436_POINT2 + 6U)
#define FT5436_POINT4 (FT5436_POINT3 + 6U)
#define FT5436_POINT5 (FT5436_POINT4 + 6U)
#define FT5436_MAX_POINT 5


typedef enum
{
    FT5436_none,
    FT5436_touch,
} FT5436_buffer_status;

typedef struct
{
    FT5436_buffer_status status;
    uint8_t numberOfTouchPoints;
}FT5436_STATUS;

typedef struct
{
    uint8_t Event_flag;
    uint16_t x;
    uint16_t id;
    uint16_t y;
} touchPoint;

extern volatile uint8_t INT_FLAG;
extern touchPoint POINT1;

extern touchPoint touch_points[FT5436_MAX_POINT];
extern FT5436_STATUS touch_status;
void Touch_INT_Callback(void);

void Touch_I2C_READTouch(touchPoint *point);
void TOUCH_INIT(void);
#endif

#endif