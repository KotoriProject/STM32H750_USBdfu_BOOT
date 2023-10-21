#include "RS485.h"
#include "usart.h"
#include "main.h"
#include "stc.h"

void RS485_test()
{
    uint8_t a[] = {
        0x55,
        0xaa,
        0x55,
        0xaa,
        0x55,
        0xaa,
        0x55,
        0xaa,
    };
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
    HAL_UART_Transmit(&huart4, a, sizeof(a), 10);
    // HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET);
}

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
// {
//     if (huart == &huart4)
//     {
//         // HAL_Delay(1);
//         HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
//         //SCB_CleanDCache_by_Addr((uint32_t *)uart4_buf,UART4_MAX_BUF);
//         HAL_HalfDuplex_EnableTransmitter(&huart4);
//         HAL_UART_Transmit_DMA(&huart4, (uint8_t *)uart4_buf, Size);
//     }
//     if (huart == &huart7)
//     {
//         HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *)uart7_buf, UART7_MAX_BUF);
//         HAL_UART_Transmit_DMA(&huart7, (uint8_t *)uart7_buf, Size);
//     }
// }

// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart == &huart4)
//     {
//         HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
//         HAL_HalfDuplex_EnableReceiver(&huart4);
//         HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t *)uart4_buf, UART4_MAX_BUF);
//     }
//     if (huart == &huart7)
//     {
//         //HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *)uart7_buf, UART7_MAX_BUF);
//     }
// }

// void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
// {
//     if (huart == &huart4)
//     {
//         __HAL_UART_CLEAR_FLAG(huart, HAL_UART_GetError(huart));
//         HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t *)uart4_buf, UART4_MAX_BUF);
//     }
//     if (huart == &huart7)
//     {
//         __HAL_UART_CLEAR_FLAG(huart, HAL_UART_GetError(huart));
//         HAL_UARTEx_ReceiveToIdle_DMA(&huart7, (uint8_t *)uart7_buf, UART7_MAX_BUF);
//     }
// }
