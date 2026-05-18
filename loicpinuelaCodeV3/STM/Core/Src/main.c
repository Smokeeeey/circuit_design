/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define VBATT_STOCK_MIN     3.5f
#define VBATT_STOCK_MAX     3.8f
#define VBATT_FULL_CHARGE   4.1f

/* ===== LEDs Active LOW ===== */

#define LED_ON   GPIO_PIN_RESET
#define LED_OFF  GPIO_PIN_SET


/* ===== Macros Debug LEDs ===== */

#define SET_LED_R(state) do {                                      \
    signal_1_0.LED_R = (state);                                    \
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin,                  \
                      (state) ? LED_ON : LED_OFF);                 \
} while(0)

#define SET_LED_G(state) do {                                      \
    signal_1_0.LED_G = (state);                                    \
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,                  \
                      (state) ? LED_ON : LED_OFF);                 \
} while(0)

#define SET_LED_B(state) do {                                      \
    signal_1_0.LED_B = (state);                                    \
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin,                  \
                      (state) ? LED_ON : LED_OFF);                 \
} while(0)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef enum
{
    STATE_NORMAL = 0,
    STATE_STOCK,
    STATE_CHARGE_STOCK,
    STATE_DISCHARGE_STOCK,
    STATE_ERROR

} System_State;

System_State system_state = STATE_NORMAL;


typedef struct {
    volatile uint32_t adc_value_ch9;
    volatile uint32_t adc_value_ch12;

    volatile float voltage_ch9_Vbatt;
    volatile float voltage_ch12_Vtemp;
} Voltage;

typedef struct
{
	/* ===== INPUTS ===== */

    bool Temperature_High;   	// PA0 -> TEMP_H
    bool Temperature_Low;    	// PA1 -> TEMP_L

    bool Batterie_High;      	// PA2 -> BATT_H
    bool Batterie_Low;       	// PA3 -> BATT_L

    bool Mode_Stock; 			// PA8 -> MODE_STOCK

    bool Charge_OK;          	// PB6 -> VChargeOK

    bool Vdisch;                  // PA5 -> Vdisch

    /* ===== OUTPUTS ===== */

    bool LED_R;                   // PA12 -> LED_R
    bool LED_B;                   // PB7 -> LED_B
    bool LED_G;                   // PB0 -> LED_G


    bool MCU_CH;                  // PB1 -> MCU_CH
    bool MCU_DISCH;               // PB4 -> MCU_DISCH



} Signal_1_0;

Voltage voltage = {0};
Signal_1_0 signal_1_0 = {0};


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	void Update_LEDs(void)
	{
	    static bool blink_state = false;

	    /* ===== RESET DEBUG STATES ===== */

	    SET_LED_R(false);
	    SET_LED_G(false);
	    SET_LED_B(false);

	    /* ===== ERROR MODE ===== */

	    if(signal_1_0.Temperature_High == false ||
	       signal_1_0.Temperature_Low  == false ||
	       signal_1_0.Batterie_High    == false ||
	       signal_1_0.Batterie_Low     == false)
	    {
	        blink_state = !blink_state;

	        if(blink_state == true)
	        {
	            SET_LED_R(true);
	            SET_LED_B(true);
	        }
	        else
	        {
	            SET_LED_R(false);
	            SET_LED_B(false);
	        }

	        SET_LED_G(false);

	        return;
	    }

	    /* ===== NORMAL STATES ===== */

	    switch(system_state)
	    {
	        case STATE_NORMAL:

	            if(signal_1_0.MCU_CH == true)
	            {
	                /* Charge normale */

	                SET_LED_R(true);
	                SET_LED_G(false);
	                SET_LED_B(false);
	            }
	            else
	            {
	                /* Batterie OK */

	                SET_LED_R(false);
	                SET_LED_G(true);
	                SET_LED_B(false);
	            }

	            break;


	        case STATE_STOCK:

	            /* Zone stockage OK */

	            SET_LED_R(false);
	            SET_LED_G(true);
	            SET_LED_B(false);

	            break;


	        case STATE_CHARGE_STOCK:

	            /* Charge stockage */

	            SET_LED_R(true);
	            SET_LED_G(false);
	            SET_LED_B(false);

	            break;


	        case STATE_DISCHARGE_STOCK:

	            /* Décharge stockage */

	            SET_LED_R(false);
	            SET_LED_G(false);
	            SET_LED_B(true);

	            break;


	        case STATE_ERROR:

	            SET_LED_R(true);
	            SET_LED_G(false);
	            SET_LED_B(true);

	            break;


	        default:

	            SET_LED_R(false);
	            SET_LED_G(false);
	            SET_LED_B(false);

	            break;
	    }
	}

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  char msg[100];
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3); // Toggle LD3

	  /* ===== ADC ===== */

	  ADC_ChannelConfTypeDef sConfig = {0};

	  /* ===== PA4 / ADC_CHANNEL_9 ===== */

	  sConfig.Channel = ADC_CHANNEL_9;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;

	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	  voltage.adc_value_ch9 = HAL_ADC_GetValue(&hadc1);

	  HAL_ADC_Stop(&hadc1);

	  /* ===== PA7 / ADC_CHANNEL_12 ===== */

	  sConfig.Channel = ADC_CHANNEL_12;

	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	  voltage.adc_value_ch12 = HAL_ADC_GetValue(&hadc1);

	  HAL_ADC_Stop(&hadc1);

	  /* ===== Conversion ===== */

	  voltage.voltage_ch9_Vbatt  = (voltage.adc_value_ch9  * 3.3f * 2) / 4095.0f;
	  voltage.voltage_ch12_Vtemp = (voltage.adc_value_ch12 * 3.3f) / 4095.0f;




	  /* ===== Délai ===== */

	  //HAL_Delay(3000); // Send every 1 second



	  /* ===== Lecture des pin 1/0 ===== */

	  /* ===== TEMP_H : PA0 ===== */

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
	  {
	      // PA0 = 1
	      signal_1_0.Temperature_High = true;
	  }
	  else
	  {
	      // PA0 = 0
	      signal_1_0.Temperature_High = false;
	  }

	  /* ----------------------------------- */

	  /* ===== TEMP_L : PA1 ===== */

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
	  {
	      // PA1 = 1
	      signal_1_0.Temperature_Low = true;
	  }
	  else
	  {
	      // PA1 = 0
	      signal_1_0.Temperature_Low = false;
	  }

	  /* ----------------------------------- */

	  /* ===== BATT_H : PA2 ===== */

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET)
	  {
	      // PA2 = 1
	      signal_1_0.Batterie_High = true;
	  }
	  else
	  {
	      // PA2 = 0
	      signal_1_0.Batterie_High = false;
	  }

	  /* ----------------------------------- */

	  /* ===== BATT_L : PA3 ===== */

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET)
	  {
	      // PA3 = 1
	      signal_1_0.Batterie_Low = true;
	  }
	  else
	  {
	      // PA3 = 0
	      signal_1_0.Batterie_Low = false;
	  }

	  /* ----------------------------------- */

	  /* ===== MODE_Stock : PA8 ===== */

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == GPIO_PIN_SET)
	  {
	      // PA8 = 1
	      signal_1_0.Mode_Stock = true;
	  }
	  else
	  {
	      // PA8 = 0
	      signal_1_0.Mode_Stock = false;
	  }

	  /* ----------------------------------- */

	  /* ===== VChargeOK : PB6 ===== */

	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET)
	  {
	      // PB6 = 1
	      signal_1_0.Charge_OK = true;
	  }
	  else
	  {
	      // PB6 = 0
	      signal_1_0.Charge_OK = false;
	  }

	  /* ===== Lecture MCU_CH : PB1 ===== */

	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
	  {
	      // PB1 = 1
	      signal_1_0.MCU_CH = true;
	  }
	  else
	  {
	      // PB1 = 0
	      signal_1_0.MCU_CH = false;
	  }

	  /* ----------------------------------- */

	  /* ===== Lecture MCU_DISCH : PB4 ===== */

	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET)
	  {
	      // PB4 = 1
	      signal_1_0.MCU_DISCH = true;
	  }
	  else
	  {
	      // PB4 = 0
	      signal_1_0.MCU_DISCH = false;
	  }




	  /* ===== Machine d'état ===== */

	  switch(system_state)
	  {
	      case STATE_NORMAL:

	          HAL_GPIO_WritePin(MCU_DISCH_GPIO_Port, MCU_DISCH_Pin, GPIO_PIN_RESET);

	          if(signal_1_0.Mode_Stock == true)
	          {
	              system_state = STATE_STOCK;
	          }
	          else
	          {
	              if(voltage.voltage_ch9_Vbatt < VBATT_FULL_CHARGE)
	              {
	                  HAL_GPIO_WritePin(MCU_CH_GPIO_Port, MCU_CH_Pin, GPIO_PIN_SET);
	              }
	              else
	              {
	                  HAL_GPIO_WritePin(MCU_CH_GPIO_Port, MCU_CH_Pin, GPIO_PIN_RESET);
	              }
	          }

	          break;


	      case STATE_STOCK:

	          if(signal_1_0.Mode_Stock == false)
	          {
	              system_state = STATE_NORMAL;
	          }
	          else if(voltage.voltage_ch9_Vbatt < VBATT_STOCK_MIN)
	          {
	              system_state = STATE_CHARGE_STOCK;
	          }
	          else if(voltage.voltage_ch9_Vbatt > VBATT_STOCK_MAX)
	          {
	              system_state = STATE_DISCHARGE_STOCK;
	          }
	          else
	          {
	              HAL_GPIO_WritePin(MCU_CH_GPIO_Port, MCU_CH_Pin, GPIO_PIN_RESET);
	              HAL_GPIO_WritePin(MCU_DISCH_GPIO_Port, MCU_DISCH_Pin, GPIO_PIN_RESET);
	          }

	          break;


	      case STATE_CHARGE_STOCK:

	          if(signal_1_0.Mode_Stock == false)
	          {
	              system_state = STATE_NORMAL;
	          }
	          else
	          {
	              HAL_GPIO_WritePin(MCU_CH_GPIO_Port, MCU_CH_Pin, GPIO_PIN_SET);
	              HAL_GPIO_WritePin(MCU_DISCH_GPIO_Port, MCU_DISCH_Pin, GPIO_PIN_RESET);

	              if(voltage.voltage_ch9_Vbatt >= VBATT_STOCK_MAX)
	              {
	                  system_state = STATE_STOCK;
	              }
	          }

	          break;


	      case STATE_DISCHARGE_STOCK:

	          if(signal_1_0.Mode_Stock == false)
	          {
	              system_state = STATE_NORMAL;
	          }
	          else
	          {
	              HAL_GPIO_WritePin(MCU_CH_GPIO_Port, MCU_CH_Pin, GPIO_PIN_RESET);
	              HAL_GPIO_WritePin(MCU_DISCH_GPIO_Port, MCU_DISCH_Pin, GPIO_PIN_SET);

	              if(voltage.voltage_ch9_Vbatt <= VBATT_STOCK_MAX)
	              {
	                  system_state = STATE_STOCK;
	              }
	          }

	          break;


	      case STATE_ERROR:

	          HAL_GPIO_WritePin(MCU_CH_GPIO_Port, MCU_CH_Pin, GPIO_PIN_RESET);
	          HAL_GPIO_WritePin(MCU_DISCH_GPIO_Port, MCU_DISCH_Pin, GPIO_PIN_RESET);

	          break;


	      default:

	          system_state = STATE_ERROR;

	          break;
	  }

	  Update_LEDs();
	  HAL_Delay(200);






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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Vdisch_GPIO_Port, Vdisch_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_G_Pin|LED_B_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, MCU_CH_Pin|LD3_Pin|MCU_DISCH_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : TEMP_H_Pin TEMP_L_Pin BATT_H_Pin BATT_L_Pin
                           MODE_CH_DISCH_Pin */
  GPIO_InitStruct.Pin = TEMP_H_Pin|TEMP_L_Pin|BATT_H_Pin|BATT_L_Pin
                          |MODE_CH_DISCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Vdisch_Pin LED_R_Pin */
  GPIO_InitStruct.Pin = Vdisch_Pin|LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_G_Pin MCU_CH_Pin LD3_Pin MCU_DISCH_Pin
                           LED_B_Pin */
  GPIO_InitStruct.Pin = LED_G_Pin|MCU_CH_Pin|LD3_Pin|MCU_DISCH_Pin
                          |LED_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configure GPIO pin : VChargeOK_Pin */
  GPIO_InitStruct.Pin = VChargeOK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VChargeOK_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
