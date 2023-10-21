#ifndef _SOFTIIC_H
#define _SOFTIIC_H

#include "stm32h7xx_hal.h"

#define ACK_OK 1  // 响应正常
#define ACK_ERR 0 // 响应错误

// IIC通信延时，Soft_IIC_Delay()函数使用，
//	通信速度在100KHz左右
#define IIC_DelayVaule 20

typedef struct
{
    uint8_t IIC_7bitADDR;
    GPIO_TypeDef *SCL_Port;
    uint16_t SCL_Pin;
    GPIO_TypeDef *SDA_Port;
    uint16_t SDA_Pin;
} SOFT_IIC_HandleTypeDef;

void Soft_IIC_Delay(uint32_t a);                                                                          // IIC延时函数
void Soft_IIC_Start(SOFT_IIC_HandleTypeDef *soft_iic);                                                    // 启动IIC通信
void Soft_IIC_Stop(SOFT_IIC_HandleTypeDef *soft_iic);                                                     // IIC停止信号
void Soft_IIC_ACK(SOFT_IIC_HandleTypeDef *soft_iic);                                                      //	发送响应信号
void Soft_IIC_NoACK(SOFT_IIC_HandleTypeDef *soft_iic);                                                    // 发送非应答信号
uint8_t Soft_IIC_WaitACK(SOFT_IIC_HandleTypeDef *soft_iic);                                               //	等待应答信号
uint8_t Soft_IIC_WriteByte(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t IIC_Data);                           // 写字节函数
uint8_t Soft_IIC_ReadByte(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t ACK_Mode);                            // 读字节函数
uint8_t Soft_IIC_ReadREG(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t Regadd, uint8_t *buf, uint16_t size);  // 读8位寄存器函数
uint8_t Soft_IIC_WriteREG(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t Regadd, uint8_t *buf, uint16_t size); // 写8位寄存器函数
#endif