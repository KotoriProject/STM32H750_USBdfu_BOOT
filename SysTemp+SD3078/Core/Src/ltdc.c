/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ltdc.c
  * @brief   This file provides code for the configuration
  *          of the LTDC instances.
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
#include "ltdc.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#include "stm32h7xx_hal_dma2d.h"
/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */
  __HAL_RCC_DMA2D_CLK_ENABLE();
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 19;
  hltdc.Init.VerticalSync = 2;
  hltdc.Init.AccumulatedHBP = 159;
  hltdc.Init.AccumulatedVBP = 22;
  hltdc.Init.AccumulatedActiveW = 1183;
  hltdc.Init.AccumulatedActiveH = 622;
  hltdc.Init.TotalWidth = 1343;
  hltdc.Init.TotalHeigh = 634;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 1024;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 600;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0x70000000;
  pLayerCfg.ImageWidth = 1024;
  pLayerCfg.ImageHeight = 600;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */


#define HBP  140	// ?????ад???????????????
#define VBP  20
#define HSW  20
#define VSW  3
#define HFP  160
#define VFP  12

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc.Init.HorizontalSync 		= HSW - 1;									// ?????ад?????????????
	hltdc.Init.VerticalSync 		= VSW	-1 ;
	hltdc.Init.AccumulatedHBP		= HBP + HSW -1;
	hltdc.Init.AccumulatedVBP 		= VBP + VSW -1;
	hltdc.Init.AccumulatedActiveW = LCD_WIDTH  + HSW + HBP -1;
	hltdc.Init.AccumulatedActiveH = LCD_HEIGTH + VSW + VBP -1;
	hltdc.Init.TotalWidth 			= LCD_WIDTH  + HSW + HBP + HFP - 1; 
	hltdc.Init.TotalHeigh 			= LCD_HEIGTH + VSW + VBP + VFP - 1;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = LCD_WIDTH;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = LCD_HEIGTH;
  pLayerCfg.PixelFormat = LCD_COLOR_DIPTH;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0x70000000;
  pLayerCfg.ImageWidth = LCD_WIDTH;
  pLayerCfg.ImageHeight = LCD_HEIGTH;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END LTDC_Init 2 */

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 24;
    PeriphClkInitStruct.PLL3.PLL3N = 330;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 10;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PB8     ------> LTDC_B6
    PG13     ------> LTDC_R0
    PE4     ------> LTDC_B0
    PE5     ------> LTDC_G0
    PE6     ------> LTDC_G1
    PB9     ------> LTDC_B7
    PG12     ------> LTDC_B4
    PG10     ------> LTDC_G3
    PD6     ------> LTDC_B2
    PD3     ------> LTDC_G7
    PA10     ------> LTDC_B1
    PI11     ------> LTDC_G6
    PA8     ------> LTDC_B3
    PH4     ------> LTDC_G4
    PC6     ------> LTDC_HSYNC
    PG7     ------> LTDC_CLK
    PG6     ------> LTDC_R7
    PF10     ------> LTDC_DE
    PC0     ------> LTDC_R5
    PA1     ------> LTDC_R2
    PA4     ------> LTDC_VSYNC
    PA2     ------> LTDC_R1
    PA6     ------> LTDC_G2
    PA5     ------> LTDC_R4
    PA3     ------> LTDC_B5
    PB1     ------> LTDC_R6
    PB0     ------> LTDC_R3
    PB11     ------> LTDC_G5
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_7|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_2
                          |GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_LTDC;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PB8     ------> LTDC_B6
    PG13     ------> LTDC_R0
    PE4     ------> LTDC_B0
    PE5     ------> LTDC_G0
    PE6     ------> LTDC_G1
    PB9     ------> LTDC_B7
    PG12     ------> LTDC_B4
    PG10     ------> LTDC_G3
    PD6     ------> LTDC_B2
    PD3     ------> LTDC_G7
    PA10     ------> LTDC_B1
    PI11     ------> LTDC_G6
    PA8     ------> LTDC_B3
    PH4     ------> LTDC_G4
    PC6     ------> LTDC_HSYNC
    PG7     ------> LTDC_CLK
    PG6     ------> LTDC_R7
    PF10     ------> LTDC_DE
    PC0     ------> LTDC_R5
    PA1     ------> LTDC_R2
    PA4     ------> LTDC_VSYNC
    PA2     ------> LTDC_R1
    PA6     ------> LTDC_G2
    PA5     ------> LTDC_R4
    PA3     ------> LTDC_B5
    PB1     ------> LTDC_R6
    PB0     ------> LTDC_R3
    PB11     ------> LTDC_G5
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_1|GPIO_PIN_0
                          |GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_10|GPIO_PIN_7
                          |GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6|GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10|GPIO_PIN_8|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_2|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/***************************************************************************************************************************************
*	Е�??? Ф�??? Е�???: LCD_CopyBuffer
*
*	Е┘╔Е▐ёЕ▐┌Ф∙�??: x - Ф╟╢Е╧ЁЕ²░Ф═┤О╪▄Е▐√Е�??╪Х▄┐Е⌡? 0~479
*			 	 y - Е·┌Г⌡╢Е²░Ф═┤О╪▄Е▐√Е�??╪Х▄┐Е⌡? 0~271
*			 	 width  - Е⌡╬Г┴┤Г └Ф╟╢Е╧ЁЕ╝╫Е╨╕О╪▄Ф°?Е╓╖Е▐√Е�???480
*				 height - Е⌡╬Г┴┤Г └Е·┌Г⌡╢Е╝╫Е╨╕О╪▄Ф°?Е╓╖Е▐√Е�???272
*				 *color - Х╕│Е╓█Е┬╤Г └Г╪⌠Е├╡Е▄╨Е°╟Е²?
*				
*	Е┤╫Ф∙╟Е┼÷Х┐�??: Е°╗Е²░Ф�??? (x,y) Х╣╥Е╖▀Е╓└Е╓█Е┬╤Г╪⌠Е├╡Е▄╨Е┬╟Ф≤╬Г╓╨Е▄╨
*
*	Х�???    Ф�???: 1. Д╫©Г■╗DMA2DЕ╝·Г▌�??
*				 2. Х╕│Г╩≤Е┬╤Г └Е▄╨Е÷÷Д╦█Х┐╫Х╤┘Х©┤Е╠▐Е╧∙Г └Ф≤╬Г╓╨Е▄╨Е�??
*				 3. Е°? lv_port_disp.c Ф√┤Д╩╤Д╦╜О╪▄Х╒╚Е┤╫Ф�??? disp_flush() Х╟┐Г■╗О╪▄Г■╗Д╩╔Е┬╥Ф√╟Ф≤╬Г╓╨Е▄╨Е÷?			 
*
*****************************************************************************************************************************************/

void LCD_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint32_t *color)
{

	DMA2D->CR	  &=	~(DMA2D_CR_START);				//	Е│°Ф╜╒DMA2D
	DMA2D->CR		=	DMA2D_M2M;							//	Е╜≤Е┌╗Е≥╗Е┬╟Е╜≤Е┌╗Е�???
	DMA2D->FGPFCCR	=	ColorMode_0;		//	Х╝╬Г╫╝И╒°Х┴╡Ф═╪Е╪�??
   DMA2D->FGOR    =  0;										// 
	DMA2D->OOR		=	LCD_WIDTH - width;				//	Х╝╬Г╫╝Х║▄Е│▐Г�??? 	
	DMA2D->FGMAR   =  (uint32_t)color;		
  // layer0 Ф╞▐Д╦╙Е┐▐Г╢═Ф�???Е█═Е╜≈Х�???

#if ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_0 ==LTDC_PIXEL_FORMAT_ARGB4444 )
	#define BytesPerPixel_0		2		//16Д╫█Х┴╡Ф╗║Е╪▐Ф╞▐Д╦╙Е┐▐Г╢═Е�???2Е╜≈Х┼┌
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
	#define BytesPerPixel_0		3		//24Д╫█Х┴╡Ф╗║Е╪▐Ф╞▐Д╦╙Е┐▐Г╢═Е�???3Е╜≈Х┼┌
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB8888
	#define BytesPerPixel_0		4		//32Д╫█Х┴╡Ф╗║Е╪▐Ф╞▐Д╦╙Е┐▐Г╢═Е�???4Е╜≈Х┼┌
#endif	
	DMA2D->OMAR		=	LCD_ADDR + BytesPerPixel_0*(LCD_WIDTH * y + x);	// Е°╟Е²─;
	DMA2D->NLR		=	(width<<16)|(height);			//	Х╝╬Е╝ И∙©Е╨╕Е▓▄Е╝╫Е�???		
	DMA2D->CR |=DMA2D_CR_TCIE; //Д╪═Х╬⌠Е╝▄Ф┬░Д╦╜Ф√╜Д╫©Х┐�??
	DMA2D->CR	  |=	DMA2D_CR_START;					//	Е░╞Е┼╗DMA2D
	//while (DMA2D->CR & DMA2D_CR_START) ;			//	Г╜┴Е╬┘Д╪═Х╬⌠Е╝▄Ф┬░

}
/* USER CODE END 1 */
