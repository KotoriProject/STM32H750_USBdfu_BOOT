/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SysTemp_TIM htim15
#define VBAT_ADC hadc1
#define ESP_UART huart6
#define I2S_I2C_SDA_Pin GPIO_PIN_3
#define I2S_I2C_SDA_GPIO_Port GPIOE
#define TF_CD_Pin GPIO_PIN_4
#define TF_CD_GPIO_Port GPIOB
#define SPI2_DC_Pin GPIO_PIN_7
#define SPI2_DC_GPIO_Port GPIOB
#define RTC_SDA_Pin GPIO_PIN_5
#define RTC_SDA_GPIO_Port GPIOD
#define I2S_I2C_SCL_Pin GPIO_PIN_13
#define I2S_I2C_SCL_GPIO_Port GPIOC
#define PWR_CTRL_Pin GPIO_PIN_8
#define PWR_CTRL_GPIO_Port GPIOI
#define RTC_SCL_Pin GPIO_PIN_4
#define RTC_SCL_GPIO_Port GPIOD
#define VBUS_sensing_Pin GPIO_PIN_9
#define VBUS_sensing_GPIO_Port GPIOA
#define SPI2_RES_Pin GPIO_PIN_3
#define SPI2_RES_GPIO_Port GPIOG
#define RS485_EN_Pin GPIO_PIN_8
#define RS485_EN_GPIO_Port GPIOF
#define EARPHONE_INPUT_Pin GPIO_PIN_2
#define EARPHONE_INPUT_GPIO_Port GPIOG
#define TOUCH_INT_Pin GPIO_PIN_6
#define TOUCH_INT_GPIO_Port GPIOH
#define TOUCH_INT_EXTI_IRQn EXTI9_5_IRQn
#define TOUCH_RST_Pin GPIO_PIN_7
#define TOUCH_RST_GPIO_Port GPIOH
#define VBUS_drive_Pin GPIO_PIN_12
#define VBUS_drive_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define APP_ADDR (0x90000000U)
//#define APP_ADDR (0x08020000U)

#define LCD_COLOR_DIPTH LTDC_PIXEL_FORMAT_RGB565
#define LCD_ADDR (0x70000000)
#define LCD_WIDTH (1024)
#define LCD_HEIGTH (600)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
