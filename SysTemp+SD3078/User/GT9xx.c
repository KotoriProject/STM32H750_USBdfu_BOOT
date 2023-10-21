#include "GT9xx.h"

#if GT9XX

#include "i2c.h"
#include "main.h"

touchPoint touch_points[GT9XX_MAX_POINT] = {0}; // 坐标点数据数组
GT9XX_STATUS touch_status = {0};

uint8_t buf[GT9XX_MAX_POINT][GT9XX_POINTS_ADDVALUE];

void TOUCH_INIT(void)
{
    NVIC_DisableIRQ(EXTI9_5_IRQn);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 速度等级
    GPIO_InitStruct.Pin = TOUCH_INT_Pin;         // 初始化 INT 引脚

    HAL_GPIO_Init(TOUCH_INT_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TOUCH_INT_GPIO_Port, TOUCH_INT_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(20);

    // GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // HAL_GPIO_Init(TOUCH_INT_GPIO_Port, &GPIO_InitStruct);
    HAL_Delay(100);
		Touch_I2C_READTouch(touch_points);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}

HAL_StatusTypeDef Touch_I2C_WRITE(uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Transmit(&hi2c2, IIC_ADD << 1, data, size, 10);
}

HAL_StatusTypeDef Touch_I2C_READ(uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Receive(&hi2c2, IIC_ADD << 1, data, size, 10);
}

HAL_StatusTypeDef Touch_REG_READ(uint16_t RegADD, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t add[2] = {RegADD >> 8, RegADD & 0xFF};
    status = Touch_I2C_WRITE(add, 2); // 写入要读取的寄存器地址
    if (status != HAL_OK)
        return status;

    return Touch_I2C_READ(data, size);
}

HAL_StatusTypeDef Touch_REG_Write(uint16_t RegADD, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;
    {
        uint8_t add[2] = {RegADD >> 8, RegADD & 0xFF};
        if (HAL_OK != Touch_I2C_WRITE(add, 2)) // 写入要读取的寄存器地址
        {
            status = HAL_ERROR;
        }
        if (HAL_OK != Touch_I2C_WRITE(data, size))
        {
            status = HAL_ERROR;
        }
    }
    return status;
}

HAL_StatusTypeDef Touch_I2C_ClearStatus()
{
    uint8_t a[] = {GT9XX_STATUS_ADD >> 8, GT9XX_STATUS_ADD & 0xff, 0};
    Touch_I2C_WRITE(a, 3);
    return HAL_OK;
}

void Touch_I2C_READTouch(touchPoint *point)
{
    //if (touch_status.status == GT9XX_touch)
    {
        Touch_REG_READ(GT9XX_STATUS_ADD, &touch_status.numberOfTouchPoints, 1);
        // 判定坐标点是否有效
        if (((touch_status.numberOfTouchPoints >> 7) & 1) == 0)
        {
            //Touch_I2C_ClearStatus();
            return;
        }
        else
        {
            touch_status.numberOfTouchPoints = (touch_status.numberOfTouchPoints & 0xF);

            for (uint8_t i = 0; i < touch_status.numberOfTouchPoints; i++)
            {
    touch_status.status = GT9XX_touch;
                Touch_REG_READ(GT9XX_POINT1 + GT9XX_POINTS_ADDVALUE * i, buf[i], GT9XX_POINTS_ADDVALUE);
                touch_points[buf[i][0]].id = buf[i][0];
                touch_points[buf[i][0]].x = ((buf[i][2]) << 8) | (buf[i][1]);
                touch_points[buf[i][0]].y = ((buf[i][4]) << 8) | (buf[i][3]);
                touch_points[buf[i][0]].size = (buf[i][6] << 8) | (buf[i][5]);
            }
            
            Touch_I2C_ClearStatus();
            if (touch_status.numberOfTouchPoints == 0)
            {
                touch_status.status = GT9XX_none;
//                for (uint8_t i = 0; i < GT9XX_MAX_POINT; i++)
//                {
//                    touch_points[i].id = 0;
//                    touch_points[i].size = 0;
//                    touch_points[i].x = 0;
//                    touch_points[i].y = 0;
//                }
            }
        }
    }
}

void Touch_INT_Callback(void)
{
    Touch_I2C_READTouch(touch_points);
}
#endif
