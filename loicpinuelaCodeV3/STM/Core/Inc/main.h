/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define TEMP_H_Pin GPIO_PIN_0
#define TEMP_H_GPIO_Port GPIOA
#define TEMP_L_Pin GPIO_PIN_1
#define TEMP_L_GPIO_Port GPIOA
#define BATT_H_Pin GPIO_PIN_2
#define BATT_H_GPIO_Port GPIOA
#define BATT_L_Pin GPIO_PIN_3
#define BATT_L_GPIO_Port GPIOA
#define Vbatt_MCU_Pin GPIO_PIN_4
#define Vbatt_MCU_GPIO_Port GPIOA
#define Vdisch_Pin GPIO_PIN_5
#define Vdisch_GPIO_Port GPIOA
#define Vtemp_Pin GPIO_PIN_7
#define Vtemp_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define MCU_CH_Pin GPIO_PIN_1
#define MCU_CH_GPIO_Port GPIOB
#define MODE_CH_DISCH_Pin GPIO_PIN_8
#define MODE_CH_DISCH_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_12
#define LED_R_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB
#define MCU_DISCH_Pin GPIO_PIN_4
#define MCU_DISCH_GPIO_Port GPIOB
#define VChargeOK_Pin GPIO_PIN_6
#define VChargeOK_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
