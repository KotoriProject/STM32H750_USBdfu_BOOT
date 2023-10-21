/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "FT5436.h"
#include "GT9xx.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demos.h"
#include "mylvgl.h"
#include "RS485.h"
#include "DMX.h"
#include "SysTemp.h"
#include "stc.h"
#include "RTC_SD3078.h"

#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart7, (uint8_t *)&ch, 1, 10); // ?¤??????????????
  return (ch);
}
// #define SDRAM_Size 16*1024*1024  //16M字节
#define SDRAM_Size 32 * 1024 * 1024 // 32M字节
#define SDRAM_BANK_ADDR (0x70000000)
/******************************************************************************************************
 *	函 数 名: SDRAM_Test
 *	入口参数: 无
 *	返 回 值: SUCCESS - 成功，ERROR - 失败
 *	函数功能: SDRAM测试
 *	说    明: 先以16位的数据宽度写入数据，再读取出来一一进行比较，随后以8位的数据宽度写入，
 *				 用以验证NBL0和NBL1两个引脚的连接是否正常。
 *******************************************************************************************************/

uint8_t SDRAM_Test(void)
{
  uint32_t i = 0;        // 计数变量
  uint32_t ReadData = 0; // 读取到的数据
  uint8_t ReadData_8b;

  uint32_t ExecutionTime_Begin; // 开始时间
  uint32_t ExecutionTime_End;   // 结束时间
  uint32_t ExecutionTime;       // 执行时间
  float ExecutionSpeed;         // 执行速度

  usb_printf("\r\n*****************************************************************************************************\r\n");
  HAL_Delay(1);
  usb_printf("\r\n进行速度测试>>>\r\n");
  HAL_Delay(1);

  // 写入 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

  for (i = 0; i < SDRAM_Size / 4; i++)
  {
    *(__IO uint32_t *)(SDRAM_BANK_ADDR + 4 * i) = (uint32_t)i; // 写入数据
  }
  ExecutionTime_End = HAL_GetTick();                                       // 获取 systick 当前时间，单位ms
  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;                 // 计算擦除时间，单位ms
  ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime * 1000; // 计算速度，单位 MB/S

  usb_printf("\r\n以32位数据宽度写入数据，大小：%d MB，耗时: %d ms, 写入速度：%.2f MB/s\r\n", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);
  HAL_Delay(1);

  // 读取	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

  for (i = 0; i < SDRAM_Size / 4; i++)
  {
    ReadData = *(__IO uint32_t *)(SDRAM_BANK_ADDR + 4 * i); // 从SDRAM读出数据
  }
  ExecutionTime_End = HAL_GetTick();                                       // 获取 systick 当前时间，单位ms
  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;                 // 计算擦除时间，单位ms
  ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime * 1000; // 计算速度，单位 MB/S

  usb_printf("\r\n读取数据完毕，大小：%d MB，耗时: %d ms, 读取速度：%.2f MB/s\r\n", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);
  HAL_Delay(1);

  // 数据校验 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  usb_printf("\r\n*****************************************************************************************************\r\n");
  HAL_Delay(1);
  usb_printf("\r\n进行数据校验>>>\r\n");
  HAL_Delay(1);

  for (i = 0; i < SDRAM_Size / 4; i++)
  {
    ReadData = *(__IO uint32_t *)(SDRAM_BANK_ADDR + 4 * i); // 从SDRAM读出数据
    if (ReadData != (uint32_t)i)                            // 检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      usb_printf("\r\nSDRAM测试失败！！\r\n");
      HAL_Delay(1);
      return ERROR; // 返回失败标志
    }
  }

  usb_printf("\r\n32位数据宽度读写通过，以8位数据宽度写入数据\r\n");
  HAL_Delay(1);
  for (i = 0; i < 255; i++)
  {
    *(__IO uint8_t *)(SDRAM_BANK_ADDR + i) = (uint8_t)i;
  }
  usb_printf("写入完毕，读取数据并比较...\r\n");
  HAL_Delay(1);
  for (i = 0; i < 255; i++)
  {
    ReadData_8b = *(__IO uint8_t *)(SDRAM_BANK_ADDR + i);
    if (ReadData_8b != (uint8_t)i) // 检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      usb_printf("8位数据宽度读写测试失败！！\r\n");
      HAL_Delay(1);
      usb_printf("请检查NBL0和NBL1的连接\r\n");
      HAL_Delay(1);
      return ERROR; // 返回失败标志
    }
  }
  usb_printf("8位数据宽度读写通过\r\n");
  HAL_Delay(1);
  usb_printf("SDRAM读写测试通过，系统正常\r\n");
  HAL_Delay(1);
  return SUCCESS; // 返回成功标志
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

extern touchPoint POINT1;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  SCB->VTOR = 0x08020000;
  __enable_irq();

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART7_Init();
  MX_I2C2_Init();
  MX_TIM4_Init();
  MX_USB_DEVICE_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
//  while (1)
//  {
//    SDRAM_Test();
//  }
  MX_LTDC_Init();
  MX_TIM14_Init();
  MX_ADC3_Init();
  MX_TIM15_Init();
  MX_UART4_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  // HAL_Delay(100);
  /*??启SysTemp和VBAT转换*/
  SysTemp_Start_Conv();

  TOUCH_INIT();
  Touch_I2C_READTouch(touch_points);
  lv_init();
  lv_port_disp_init();
  // lv_disp_set_rotation(NULL, LV_DISP_ROT_NONE);
  lv_port_indev_init();
  mylv_DMX_Controller();
  // lv_demo_music();
  //  lv_demo_widgets();
  // lv_demo_benchmark();
  lv_task_handler();

  HAL_UARTEx_ReceiveToIdle_DMA(&ESP_UART, uart7_buf, UART7_MAX_BUF);

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_Delay(50);
  for (uint16_t i = 0; i < 0x3F; i++)
  {
    BL_change(i);
    HAL_Delay(1);
  }
  DMX_Init(&htim14);
  HAL_TIM_Base_Start_IT(&htim14);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    static uint16_t count = 0;
    lv_task_handler();
    // HAL_Delay(5);
    lv_timer_handler();

    // HAL_ADC_Start(&hadc1);
    // HAL_ADC_PollForConversion(&hadc1, 10);
    // float vbat = (float)HAL_ADC_GetValue(&hadc1) / 0xFFFF * 3.3 * 2;
    // USB_Transmit_Printf("BAT Voltage: %0.3f", vbat);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
   */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC | RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 12;
  PeriphClkInitStruct.PLL2.PLL2N = 120;
  PeriphClkInitStruct.PLL2.PLL2P = 4;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 1;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_1;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x70000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_1MB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
