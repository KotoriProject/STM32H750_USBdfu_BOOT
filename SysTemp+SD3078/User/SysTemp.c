#include "SysTemp.h"

#include "adc.h"
#include "tim.h"
#include "stc.h"

#define Temp30_check_value (*(uint16_t *)0x1FF1E820)
#define Temp110_check_value (*(uint16_t *)0x1FF1E840)

void SysTemp_Start_Conv()
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start(&SysTemp_TIM);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)SysTemp_buf, sizeof(SysTemp_buf) / sizeof(*SysTemp_buf));
    HAL_ADC_Start_DMA(&VBAT_ADC, (uint32_t *)VBAT_buf, sizeof(VBAT_buf) / sizeof(*VBAT_buf));
}

float SysTemp_GET()
{
    double AD_value_avg;
    SCB_CleanDCache_by_Addr((uint32_t *)SysTemp_buf, sizeof(SysTemp_buf) / sizeof(*SysTemp_buf));
    for (uint8_t i = 0; i < sizeof(SysTemp_buf) / sizeof(*SysTemp_buf); i++)
    {
        AD_value_avg += SysTemp_buf[i];
    }
    AD_value_avg /= sizeof(SysTemp_buf) / sizeof(*SysTemp_buf); // 求平均值

    return ((double)80 / (Temp110_check_value - Temp30_check_value)) * (AD_value_avg - Temp30_check_value) + 30; //	计算温度
}

#define VBAT_MAX 4.2
#define VBAT_MIN 3.0

float VBAT_GET()
{
    double AD_value_avg;
    SCB_CleanDCache_by_Addr((uint32_t *)VBAT_buf, sizeof(VBAT_buf) / sizeof(*VBAT_buf));
    for (uint8_t i = 0; i < sizeof(VBAT_buf) / sizeof(*VBAT_buf); i++)
    {
        AD_value_avg += VBAT_buf[i];
    }
    AD_value_avg /= sizeof(VBAT_buf) / sizeof(*VBAT_buf); // 求平均值

    return (float)AD_value_avg / 65536 * 3.3 * 2;
}

uint8_t VBAT_GET_PCT()
{
    return (VBAT_GET() - VBAT_MIN) / (VBAT_MAX - VBAT_MIN) * 100;
}