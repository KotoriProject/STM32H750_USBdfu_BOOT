#include "DMX.h"

#include "tim.h"
#include "usart.h"
#include "stc.h"

uint16_t dmx_real_chann = 512;
uint16_t dmx_MTBP_time = 0;
void DMX_Init(TIM_HandleTypeDef *htim)
{
    /*设定定时器的ARR为Break时间*/
    htim->Instance->ARR = 88;

    /*初始化dmx_buf*/
    for (uint32_t i = 0; i < sizeof(dmx_buf); i++)
    {
        dmx_buf[i] = 0;
    }
    /*使能RS485芯片发送*/
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
}

void DMX_DATA_CHANGE(uint16_t channel ,uint8_t data)
{
    dmx_buf[channel]=data;
}

typedef enum
{
    DMX_BREAK,
    DMX_MAB,
    DMX_DATA,
    DMX_MTBP,
} DMX_STATUS;

void DMX_Task(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    static DMX_STATUS dmx_status = DMX_BREAK;
    HAL_TIM_Base_Stop_IT(htim);
    if (dmx_status == DMX_BREAK)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        /*拉低电平发送Break信号并保持88us*/
        dmx_status = DMX_MAB;
        htim->Instance->ARR = 88;
    }
    else if (dmx_status == DMX_MAB)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        /*拉高电平发送MAB信号并保持8su*/
        dmx_status = DMX_DATA;
        htim->Instance->ARR = 8;
    }
    else if (dmx_status == DMX_DATA)
    {
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*DMA方式发送数据*/
        HAL_UART_Transmit_DMA(&huart4, dmx_buf, dmx_real_chann);
        if (dmx_MTBP_time != 0)
            dmx_status = DMX_MTBP;
        else
            dmx_status = DMX_BREAK;
        htim->Instance->ARR = 44 * dmx_real_chann + 1000;
    }
    else if (dmx_status == DMX_MTBP)
    {
        htim->Instance->ARR = dmx_MTBP_time;
        dmx_status = DMX_BREAK;
    }
    HAL_TIM_Base_Start_IT(htim);
}
