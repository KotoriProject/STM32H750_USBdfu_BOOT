#include "CircuQueue.h"

#include "usart.h"
#include "stc.h"

#include "usbd_cdc_if.h"

// volatile uint8_t USB_Trans_Flag = 0;   // USB发送标志
// volatile uint16_t trans_pointer = 0;   // 要发送的数据的指针
// volatile uint16_t wait2trans_size = 0; // 等待发送的总长度

// uint8_t CircuQueue_Transmit_port(uint8_t *data_buf, uint32_t size)
// {
//     CDC_Transmit_FS(data_buf,size); //
//     return 0;
// }

// void CircuQueue_Rec_Callback(uint8_t *data, uint32_t size)
// {
// }

// void CircuQueue_Trans_Callback()
// {
//     if (wait2trans_size == 0)
//     {
//         USB_Trans_Flag = 0;
//     }
//     else
//     {
//         if ((trans_pointer + wait2trans_size) <= UART7_MAX_BUF)                   // 未超过环形区
//         {                                                                         //
//             SCB_InvalidateDCache_by_Addr(uart7_buf, UART7_MAX_BUF);               // 清除Dcache，让CPU直接从memory中读取
//             CircuQueue_Transmit_port(&uart7_buf[trans_pointer], wait2trans_size); //
//             trans_pointer += wait2trans_size;                                     // 队列指针
//             wait2trans_size = 0;
//         }
//         else // 超出环形区
//         {
//             SCB_InvalidateDCache_by_Addr(uart7_buf, UART7_MAX_BUF);
//             CDC_Transmit_FS(&uart7_buf[trans_pointer], UART7_MAX_BUF - trans_pointer);
//             trans_pointer = 0;
//             wait2trans_size -= (UART7_MAX_BUF - trans_pointer); // 剩余待发送队列
//         }
//     }
// }

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
// {
//     static uint16_t bef_size = 0;                      // 保存上一次接收的数组结尾指针+1（即为本次接收的起始地址）
//     uint16_t real_size = 0;                            //
//     if (Size > bef_size)                               // 未溢出
//     {                                                  //
//         real_size = Size - bef_size;                   // 真正接收到的数据长度
//     }                                                  //
//     else                                               // 溢出
//     {                                                  //
//         real_size = (UART7_MAX_BUF - bef_size) + Size; // 溢出情况下，真正的数据长度需要环形计算
//     }
//     if (USB_Trans_Flag == 0)                                                           // USB未处于发送状态
//     {                                                                                  //
//         USB_Trans_Flag = 1;                                                            // USB发送标志
//         SCB_InvalidateDCache_by_Addr(uart7_buf, UART7_MAX_BUF);                        // 因为是使用DMA接收数据，CPU读之前先清Dcache
//         if ((trans_pointer + real_size) > UART7_MAX_BUF)                               // 溢出
//         {                                                                              //
//             CDC_Transmit_FS(&uart7_buf[trans_pointer], UART7_MAX_BUF - trans_pointer); // 发送未溢出部分
//             wait2trans_size = trans_pointer + real_size - UART7_MAX_BUF;               // 剩余待发送队列
//             trans_pointer = 0;                                                         // 重置队列指针
//         }                                                                              //
//         else                                                                           // 未溢出
//         {                                                                              //
//             CDC_Transmit_FS(&uart7_buf[trans_pointer], real_size);                     // 发送所有接受到的数据
//             trans_pointer = Size;                                                      // 使用Size纠正可能因USB发送不及时到导致错误的指针
//         }
//     }
//     else                              // USB处于发送状态
//     {                                 //
//         wait2trans_size += real_size; // 在USB处于发送时，只将数据加入待发送队列
//     }
//     bef_size = Size; // 每次刷新DMA数组指针的位置
//                      // HAL_UARTEx_ReceiveToIdle_DMA(&ESP_UART, uart7_buf, 100);
// }
