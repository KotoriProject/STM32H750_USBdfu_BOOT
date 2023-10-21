#include "Soft_iic.h"

void Soft_IIC_SDA(SOFT_IIC_HandleTypeDef *soft_iic, GPIO_PinState statu)
{
    HAL_GPIO_WritePin(soft_iic->SDA_Port, soft_iic->SDA_Pin, statu);
}

void Soft_IIC_SCL(SOFT_IIC_HandleTypeDef *soft_iic, GPIO_PinState statu)
{
    HAL_GPIO_WritePin(soft_iic->SCL_Port, soft_iic->SCL_Pin, statu);
}

/*****************************************************************************************
 *	函 数 名: Soft_IIC_Delay
 *	入口参数: a - 延时时间
 *	返 回 值: 无
 *	函数功能: 简单延时函数
 *	说    明: 为了移植的简便性且对延时精度要求不高，所以不需要使用定时器做延时
 ******************************************************************************************/

void Soft_IIC_Delay(uint32_t a)
{
    volatile uint16_t i;
    while (a--)
    {
        for (i = 0; i < 8; i++)
            ;
    }
}

/*****************************************************************************************
 *	函 数 名: Soft_IIC_Start
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC起始信号
 *	说    明: 在SCL处于高电平期间，SDA由高到低跳变为起始信号
 ******************************************************************************************/

void Soft_IIC_Start(SOFT_IIC_HandleTypeDef *soft_iic)
{
    Soft_IIC_SDA(soft_iic, GPIO_PIN_SET);
    Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);

    Soft_IIC_SDA(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: Soft_IIC_Stop
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC停止信号
 *	说    明: 在SCL处于高电平期间，SDA由低到高跳变为起始信号
 ******************************************************************************************/

void Soft_IIC_Stop(SOFT_IIC_HandleTypeDef *soft_iic)
{
    Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SDA(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);

    Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SDA(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: Soft_IIC_ACK
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC应答信号
 *	说    明: 在SCL为高电平期间，SDA引脚输出为低电平，产生应答信号
 ******************************************************************************************/

void Soft_IIC_ACK(SOFT_IIC_HandleTypeDef *soft_iic)
{
    Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SDA(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);

    Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET); // SCL输出低时，SDA应立即拉高，释放总线
    Soft_IIC_SDA(soft_iic, GPIO_PIN_SET);

    Soft_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: Soft_IIC_NoACK
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC非应答信号
 *	说    明: 在SCL为高电平期间，若SDA引脚为高电平，产生非应答信号
 ******************************************************************************************/

void Soft_IIC_NoACK(SOFT_IIC_HandleTypeDef *soft_iic)
{
    Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SDA(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);

    Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
    Soft_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: Soft_IIC_WaitACK
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: 等待接收设备发出应答信号
 *	说    明: 在SCL为高电平期间，若检测到SDA引脚为低电平，则接收设备响应正常
 ******************************************************************************************/

uint8_t Soft_IIC_WaitACK(SOFT_IIC_HandleTypeDef *soft_iic)
{
    Soft_IIC_SDA(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);
    Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
    Soft_IIC_Delay(IIC_DelayVaule);

    if (HAL_GPIO_ReadPin(soft_iic->SDA_Port, soft_iic->SDA_Pin) != GPIO_PIN_RESET) // 判断设备是否有做出响应
    {
        Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
        Soft_IIC_Delay(IIC_DelayVaule);
        return ACK_ERR; // 无应答
    }
    else
    {
        Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
        Soft_IIC_Delay(IIC_DelayVaule);
        return ACK_OK; // 应答正常
    }
}

/*****************************************************************************************
 *	函 数 名:	Soft_IIC_WriteByte
 *	入口参数:	IIC_Data - 要写入的8位数据
 *	返 回 值:	ACK_OK  - 设备响应正常
 *          	ACK_ERR - 设备响应错误
 *	函数功能:	写一字节数据
 *	说    明:高位在前
 ******************************************************************************************/

uint8_t Soft_IIC_WriteByte(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t IIC_Data)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        Soft_IIC_SDA(soft_iic, IIC_Data & 0x80);

        Soft_IIC_Delay(IIC_DelayVaule);
        Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
        Soft_IIC_Delay(IIC_DelayVaule);
        Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
        if (i == 7)
        {
            Soft_IIC_SDA(soft_iic, GPIO_PIN_SET);
        }
        IIC_Data <<= 1;
    }

    return Soft_IIC_WaitACK(soft_iic); // 等待设备响应
}

/*****************************************************************************************
 *	函 数 名:	Soft_IIC_ReadByte
 *	入口参数:	ACK_Mode - 响应模式，输入1则发出应答信号，输入0发出非应答信号
 *	返 回 值:	ACK_OK  - 设备响应正常
 *          	ACK_ERR - 设备响应错误
 *	函数功能:读一字节数据
 *	说    明:1.高位在前
 *				2.应在主机接收最后一字节数据时发送非应答信号
 ******************************************************************************************/

uint8_t Soft_IIC_ReadByte(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t ACK_Mode)
{
    uint8_t IIC_Data = 0;
    uint8_t i = 0;

    for (i = 0; i < 8; i++)
    {
        IIC_Data <<= 1;

        Soft_IIC_SCL(soft_iic, GPIO_PIN_SET);
        Soft_IIC_Delay(IIC_DelayVaule);
        IIC_Data |= (HAL_GPIO_ReadPin(soft_iic->SDA_Port, soft_iic->SDA_Pin) & 0x01);
        Soft_IIC_SCL(soft_iic, GPIO_PIN_RESET);
        Soft_IIC_Delay(IIC_DelayVaule);
    }

    if (ACK_Mode == 1) //	应答信号
        Soft_IIC_ACK(soft_iic);
    else
        Soft_IIC_NoACK(soft_iic); // 非应答信号

    return IIC_Data;
}

uint8_t Soft_IIC_ReadREG(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t Regadd, uint8_t *buf, uint16_t size)
{
    Soft_IIC_Start(soft_iic);                                               // 发送IIC起始信号
    if (Soft_IIC_WriteByte(soft_iic, soft_iic->IIC_7bitADDR << 1 | 0) == 0) // 发送7位iic地址|写模式
    {                                                                       //
        Soft_IIC_Stop(soft_iic);                                            // 未检测到应答，通信失败返回0
        return 0;
    }
    if (Soft_IIC_WriteByte(soft_iic, Regadd) == 0) // 发送要读取的8位寄存器地址
    {
        Soft_IIC_Stop(soft_iic);
        return 0;
    }
    Soft_IIC_Start(soft_iic);                                               // 重新发送IIC起始信号改变通信方向
    if (Soft_IIC_WriteByte(soft_iic, soft_iic->IIC_7bitADDR << 1 | 1) == 0) // 发送7位iic地址|读模式
    {
        Soft_IIC_Stop(soft_iic);
        return 0;
    }
    for (uint16_t i = 0; i < size; i++)          // 连续读取size大小的数据
    {                                            //
        buf[i] = Soft_IIC_ReadByte(soft_iic, 1); // 将读取的数据传入目标数组
    }
    Soft_IIC_Stop(soft_iic); // 发送IIC停止信号
    return 1;
}

uint8_t Soft_IIC_WriteREG(SOFT_IIC_HandleTypeDef *soft_iic, uint8_t Regadd, uint8_t *buf, uint16_t size)
{
    Soft_IIC_Start(soft_iic);                                               // 发送IIC起始信号
    if (Soft_IIC_WriteByte(soft_iic, soft_iic->IIC_7bitADDR << 1 | 0) == 0) // 发送7位iic地址|写模式
    {                                                                       //
        Soft_IIC_Stop(soft_iic);                                            // 未检测到应答，通信失败返回0
        return 0;
    }
    if (Soft_IIC_WriteByte(soft_iic, Regadd) == 0) // 发送要写入的8位寄存器地址
    {
        Soft_IIC_Stop(soft_iic);
        return 0;
    }
    // Soft_IIC_Start(soft_iic);                                               // 重新发送IIC起始信号改变通信方向
    // if (Soft_IIC_WriteByte(soft_iic, soft_iic->IIC_7bitADDR << 1 | 1) == 0) // 发送7位iic地址|读模式
    // {
    //     Soft_IIC_Stop(soft_iic);
    //     return 0;
    // }
    for (uint16_t i = 0; i < size; i++)                // 连续读取size大小的数据
    {                                                  //
        if (Soft_IIC_WriteByte(soft_iic, buf[i]) == 0) // 发送要写入的8位寄存器地址
        {
            Soft_IIC_Stop(soft_iic);
            return 0;
        }
    }
    Soft_IIC_Stop(soft_iic); // 发送IIC停止信号
    return 1;
}
