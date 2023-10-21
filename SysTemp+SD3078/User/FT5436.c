#include "FT5436.h"
#include "i2c.h"
#if FT5436

volatile uint8_t INT_FLAG = 0;

touchPoint touch_points[FT5436_MAX_POINT] = {0};    //坐标点数据数组
FT5436_STATUS touch_status = {FT5436_none}; //触摸状态

uint8_t buf[FT5436_MAX_POINT][4];
HAL_StatusTypeDef Touch_I2C_WRITE(uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Transmit(&hi2c2, IIC_ADD << 1, data, size, 10);
}

HAL_StatusTypeDef Touch_I2C_READ(uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Receive(&hi2c2, IIC_ADD << 1, data, size, 10);
}

HAL_StatusTypeDef Touch_REG_READ(uint8_t RegADD, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = Touch_I2C_WRITE(&RegADD, 1); // 写入要读取的寄存器地址
    if (status != HAL_OK)
        return status;

    return Touch_I2C_READ(data, size);
}

HAL_StatusTypeDef Touch_REG_Write(uint8_t RegADD, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;
    {
        Touch_I2C_WRITE(&RegADD, 1); // 写入要读取的寄存器地址
        if (HAL_OK != Touch_I2C_WRITE(data, size))
        {
            status = HAL_ERROR;
        }
    }
    return status;
}
HAL_StatusTypeDef Touch_I2C_ClearStatus()
{
    return HAL_OK;
}

void Touch_I2C_READTouch(touchPoint *point)
{
    if (touch_status.status == FT5436_touch)
    {
        Touch_REG_READ(FT5436_STATUS_ADD, &touch_status.numberOfTouchPoints, 1);
        if (touch_status.numberOfTouchPoints > FT5436_MAX_POINT)
            touch_status.numberOfTouchPoints = FT5436_MAX_POINT;
        for (uint8_t i = 0; i < touch_status.numberOfTouchPoints; i++)
        {
            Touch_REG_READ(FT5436_POINT1 + 6 * i, buf[i], 4);
            uint8_t id = (buf[i][2] >> 4) & 0xF;
            touch_points[id].Event_flag = (buf[i][0] >> 6) & 0x3;
            touch_points[id].x = ((buf[i][1] & 0xF) << 8) | (buf[i][0]);
            touch_points[id].id = id;
            touch_points[id].y = ((buf[i][3] & 0xF) << 8) | (buf[i][2]);
        }
        if (touch_status.numberOfTouchPoints == 0)
        {
            touch_status.status = FT5436_none;
            for (uint8_t i = 0; i < FT5436_MAX_POINT; i++)
            {
                touch_points[i].Event_flag = 0;
                touch_points[i].x = 0;
                touch_points[i].id = 0;
                touch_points[i].y = 0;
            }
        }
    }
}

void Touch_INT_Callback()
{
    touch_status.status = FT5436_touch;
    Touch_I2C_READTouch(touch_points);
}
void TOUCH_INIT()
{
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(200);
    uint8_t a[1] = {0};
    if (HAL_OK != Touch_REG_Write(0x00, a, 1))
    {
    }
}

#endif
