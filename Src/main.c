/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PWR/PWR_STOP1/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U0xx PWR HAL API to enter
  * and exit the STOP 1 mode.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "stm32u0xx_ll_lptim.h"
//#include "stm32u0xx_hal_lptim.h"
#include "stm32u0xx_nucleo.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

LPTIM_HandleTypeDef hlptim1;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define TRACSENS_CFG_AUTORELOAD_VALUE 0xFFFF

#define LED_TOGGLE_DELAY         100
static __IO uint32_t TimingDelay;


typedef enum
{
	NORMAL_CounterMode,
	INVERT_CounterMode,
}PULSER_CounterMode_t;

typedef enum
{
	UNKNOWN_CounterDirection,
	FORWARD_CounterDirection,
	BACKWARD_CounterDirection,
}PULSER_CounterDirection_t;

static PULSER_CounterMode_t eMode= NORMAL_CounterMode;
static uint16_t uwCompareValue;

static PULSER_CounterDirection_t eCounterDirection= UNKNOWN_CounterDirection;
// This variable acts as the upper 16-bits of our 32-bit counter.
// It's incremented on overflow and decremented on underflow.
static volatile int32_t lCntrMultiplier= 0;

// Variables to store the cumulative forward and backward counts
static int32_t lForwardCount = 0;
static int32_t lBackwardCount = 0;
// Stores the last known absolute counter value to calculate the delta
static int32_t lLastCounterValue = 0;

static int32_t lCntrErrorReading= 0;

typedef enum
{
	NONE_CounterErrorState= 0,
	FWD_EXPECTING_BWD_CounterErrorState,
	BWD_EXPECTING_FWD_CounterErrorState,
	FWD_EXPECTING_BWD_END_CounterErrorState,
	BWD_EXPECTING_FWD_END_CounterErrorState,
}TRACSENS_CounterErrorState_t;

typedef struct
{
	bool enableErrorPatternCheck;
	bool useCompensatedValue;
	uint16_t errorPatternConfirmationCount;

	int32_t rteOffsetValue;
	int32_t rteLastSavedValue;
	uint32_t rteErrorPatternCount;
	bool rteErrorPatternCompensationStarted;
	TRACSENS_CounterErrorState_t rteErrorPatternState;
	int32_t rteErrorPatternPreviousPulse;
	bool rteErrorPatternJustStarted;
}TRACSENS_t;

static TRACSENS_t tracsense_config; // Static instance
static TRACSENS_t *pConfig = &tracsense_config; // Pointer to the instance


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
static void SYSCLKConfig_STOP(void);

void TRACSENS_StartCounting();
void TRACSENS_DisplayInfo(void);
static int32_t TRACSENS_GetCounter(void);
void TRACSENS_CompareCallback(LPTIM_HandleTypeDef *hlptim);
void TRACSENS_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim);
void TRACSENS_CounterChangedToUpCallback(LPTIM_HandleTypeDef *hlptim);
void TRACSENS_CounterChangedToDownCallback(LPTIM_HandleTypeDef *hlptim);
void TRACSENS_ChangedToUpErrorHandling(void);
void TRACSENS_ChangedToDownErrorHandling(void);
static void TRACSENS_Power(bool _enable);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  pConfig->enableErrorPatternCheck = false; // Initialize to a known state
  /* Configure LED4 */
  BSP_LED_Init(LED4);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* User push-button (EXTI_Line13) will be used to wakeup the system from Stop mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_LPTIM1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Ensure that MSI is wake-up system clock */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

  //==========================================================
  TRACSENS_StartCounting();
  
	if (HAL_LPTIM_Counter_Start_IT(&hlptim1) != HAL_OK)
	{
    Error_Handler();
	}
  
	/* Disable autoreload write complete interrupt */
	// __HAL_LPTIM_DISABLE_IT(&hlptim1, LPTIM_IT_ARROK);
  
	// uint32_t value = LL_LPTIM_OC_GetCompareCH1(LPTIM1);
  
  // UART_Printf("main start %ld\r\n",value);
  UART_Printf("main start \r\n");
  
  // Initialize the last counter value on startup
  lLastCounterValue = TRACSENS_GetCounter();
  // TRACSENS_DisplayInfo();
  //==========================================================

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Insert 5 second delay */
    TRACSENS_DisplayInfo();
    HAL_Delay(3000);
    /* Turn off LED4 */
    BSP_LED_Off(LED4);

  /* Suspend tick */
   HAL_SuspendTick();
   HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

    /* ... STOP 1 mode ... */

    /* Re-configure the system clock to 48 MHz based on MSI, enable and
       select PLL as system clock source (PLL is disabled in STOP mode) */

    SYSCLKConfig_STOP();
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.UltraLowPowerClock.Polarity = LPTIM_CLOCKPOLARITY_RISING;
  hlptim1.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.Period = 65535;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_EXTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim1.Init.RepetitionCounter = 0;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  hrtc.Init.BinMode = RTC_BINARY_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUTTON_USER_Pin */
  GPIO_InitStruct.Pin = BUTTON_USER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_USER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//===========================================================

/**
  * @brief  Calculates and displays the current, cumulative forward, and cumulative backward counts.
  * @note   This function calculates the change (delta) in position since its last execution.
  * It then updates the respective forward or backward cumulative counters.
  * @param  None
  * @retval None
  */
void TRACSENS_DisplayInfo(void)
{
	// Get the current absolute 32-bit position
	int32_t current_value = TRACSENS_GetCounter();
	
	// Calculate the change in position since the last time this function was called
	int32_t delta = current_value - lLastCounterValue;
	
	if (delta > 0)
	{
		// If delta is positive, the movement was forward
		lForwardCount += delta;
	}
	else if (delta < 0)
	{
		// If delta is negative, the movement was backward. Add the absolute value.
		lBackwardCount += -delta;
	}
	
	// Update the last counter value for the next calculation
	lLastCounterValue = current_value;
	
	UART_Printf("Current: %ld, Forward: %ld, Backward: %ld\r\n",
			current_value, lForwardCount, lBackwardCount);
}

/**
  * @brief  Gets the full 32-bit encoder counter value.
  * @note   This function atomically reads the hardware counter and software multiplier
  * to prevent race conditions, returning the combined absolute position.
  * @param  None
  * @retval The 32-bit signed encoder position.
  */
static int32_t TRACSENS_GetCounter(void)
{
	uint16_t hw_counter_val;
	int32_t multiplier_val;
	int32_t full_counter_val;

	/* Reading a hardware register while an interrupt can modify a related variable (lCntrMultiplier)
	 * can lead to a race condition. Disable interrupts briefly to ensure an atomic read of both values.
	 */
	__disable_irq();

	/* Read the 16-bit hardware counter and the 32-bit software multiplier */
	hw_counter_val = LL_LPTIM_GetCounter(LPTIM1);
	multiplier_val = lCntrMultiplier;

	/* Re-enable interrupts */
	__enable_irq();

	/* Combine the multiplier (upper bits) and the hardware counter (lower bits)
	 * to get the full 32-bit position. (TRACSENS_CFG_AUTORELOAD_VALUE is 0xFFFF, so +1 gives 65536)
	 */
	full_counter_val = (multiplier_val * (TRACSENS_CFG_AUTORELOAD_VALUE + 1)) + hw_counter_val;

    return full_counter_val;
}

void TRACSENS_CompareCallback(LPTIM_HandleTypeDef *hlptim)
{
	UART_Printf("TRACSENS_CompareCallback\n\r");
	if(UNKNOWN_CounterDirection== eCounterDirection)/*initially we don't know. we choose forward cos this compare confirming it forward*/
	{
		eCounterDirection= FORWARD_CounterDirection;
	}
}

/**
  * @brief  LPTIM Counter direction change to UP callback in Interrupt context.
  * @note   This function is called when the encoder direction changes to forward (counting up).
  * It's responsible for setting the global direction state.
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void TRACSENS_CounterChangedToUpCallback(LPTIM_HandleTypeDef *hlptim)
{
	/* The hardware has detected the counting direction is now UP (forward). */
	/* We record this state so the AutoReload callback knows how to handle a wrap-around. */
	eCounterDirection = FORWARD_CounterDirection;

	if(true == pConfig->enableErrorPatternCheck)
	{
		TRACSENS_ChangedToUpErrorHandling();
	}
	UART_Printf("Direction changed to UP (Forward)\r\n"); // Optional: can be noisy
}

/**
  * @brief  LPTIM Counter direction change to DOWN callback in Interrupt context.
  * @note   This function is called when the encoder direction changes to backward (counting down).
  * It's responsible for setting the global direction state.
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void TRACSENS_CounterChangedToDownCallback(LPTIM_HandleTypeDef *hlptim)
{
	/* The hardware has detected the counting direction is now DOWN (backward). */
	/* We record this state so the AutoReload callback knows how to handle a wrap-around. */
	eCounterDirection = BACKWARD_CounterDirection;

	if(true == pConfig->enableErrorPatternCheck)
	{
		TRACSENS_ChangedToDownErrorHandling();
	}
	UART_Printf("Direction changed to DOWN (Backward)\r\n"); // Optional: can be noisy
}

/**
  * @brief  LPTIM Auto-Reload Match callback in Interrupt context.
  * @note   This function is called when the LPTIM counter overflows (counts past 0xFFFF)
  * or underflows (counts below 0x0000). It extends the 16-bit hardware
  * counter to a 32-bit software counter using lCntrMultiplier.
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void TRACSENS_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
  /* This callback is triggered when the counter wraps around.
   * We check the last known direction, set by the Up/Down callbacks,
   * to determine if it was an overflow or an underflow.
   */
  int32_t current_value;
  if (eCounterDirection == FORWARD_CounterDirection)
  {
    /* OVERFLOW: If counting forward, a wrap-around from 0xFFFF to 0x0000 has occurred.
     * Increment the multiplier to extend the counter's upper bits.
     */
    lCntrMultiplier++;
  }
  else if (eCounterDirection == BACKWARD_CounterDirection)
  {
    /* UNDERFLOW: If counting backward, a wrap-around from 0x0000 to 0xFFFF has occurred.
     * Decrement the multiplier.
     */
    lCntrMultiplier--;
  }else
  {
    current_value = TRACSENS_GetCounter();
	
    if (current_value != 0)
    {
      // 65535 = underflow
      lCntrMultiplier--;
      eCounterDirection= BACKWARD_CounterDirection; 
    }
    else
    {
      // 0 = overflow
      lCntrMultiplier++;
      eCounterDirection= FORWARD_CounterDirection; 
    }
  }

  UART_Printf("AutoReloadMatchCallback: Dir=%d, Multiplier=%ld, curr=%ld\r\n", eCounterDirection, lCntrMultiplier, current_value); // Optional: can be noisy
}

void TRACSENS_StartCounting()
{
	TRACSENS_Power(true);

	if(NORMAL_CounterMode== eMode)
	{
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_COMPARE_MATCH_CB_ID, TRACSENS_CompareCallback);
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_AUTORELOAD_MATCH_CB_ID, TRACSENS_AutoReloadMatchCallback);
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_DIRECTION_UP_CB_ID, TRACSENS_CounterChangedToUpCallback);
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_DIRECTION_DOWN_CB_ID, TRACSENS_CounterChangedToDownCallback);
	}
	else if(INVERT_CounterMode== eMode)
	{
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_COMPARE_MATCH_CB_ID, TRACSENS_CompareCallback);
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_AUTORELOAD_MATCH_CB_ID, TRACSENS_AutoReloadMatchCallback);
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_DIRECTION_UP_CB_ID, TRACSENS_CounterChangedToDownCallback);
		HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_DIRECTION_DOWN_CB_ID, TRACSENS_CounterChangedToUpCallback);
	}

	uwCompareValue=0x01;/*used once to detect direction*/

	LL_LPTIM_EnableIT_ARRM(LPTIM1); 	/*Enable autoreload match interrupt (ARRMIE).*/
	LL_LPTIM_EnableIT_UP(LPTIM1);		/*Enable direction change to up interrupt (UPIE).*/
	LL_LPTIM_EnableIT_DOWN(LPTIM1);		/*Enable direction change to down interrupt (DOWNIE).*/

	LL_LPTIM_OC_SetCompareCH1(LPTIM1, 11);

	LL_LPTIM_SetEncoderMode(LPTIM1, LL_LPTIM_ENCODER_MODE_RISING_FALLING);
    LL_LPTIM_EnableEncoderMode(LPTIM1);
    LL_LPTIM_Enable(LPTIM1);
	LL_LPTIM_SetAutoReload(LPTIM1, TRACSENS_CFG_AUTORELOAD_VALUE);
    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS);

    /*this is needed during power up as we always get extra pulse a bit while after start counting*/
	HAL_Delay(1);	/*when reboot, we get extra pulse*/

	do/* 2 consecutive readings need to be the same*/
    {
    	lCntrErrorReading= LL_LPTIM_GetCounter(LPTIM1);
    }while(LL_LPTIM_GetCounter(LPTIM1)!= lCntrErrorReading);
}

void TRACSENS_ChangedToUpErrorHandling(void)
{
	if(BWD_EXPECTING_FWD_CounterErrorState== pConfig->rteErrorPatternState)
	{
		pConfig->rteErrorPatternState= FWD_EXPECTING_BWD_END_CounterErrorState;
	}

}

void TRACSENS_ChangedToDownErrorHandling(void)
{
	int32_t _curr= TRACSENS_GetCounter();
	if(0!= (_curr- pConfig->rteErrorPatternPreviousPulse))
	{
		pConfig->rteErrorPatternState= NONE_CounterErrorState; /*reset if we get real pulse inbetween error pattern*/
		pConfig->rteErrorPatternJustStarted= false;
		if(false== pConfig->rteErrorPatternCompensationStarted)
		{
			pConfig->rteErrorPatternCount= 0;/*we need consecutive error pattern to mark the meter as erroneous that we can handle*/
		}
	}
	if(NONE_CounterErrorState== pConfig->rteErrorPatternState)
	{
		pConfig->rteErrorPatternJustStarted= true;
		pConfig->rteErrorPatternState= BWD_EXPECTING_FWD_CounterErrorState;
	}
	else if(FWD_EXPECTING_BWD_CounterErrorState== pConfig->rteErrorPatternState)
	{
		pConfig->rteErrorPatternState= BWD_EXPECTING_FWD_END_CounterErrorState;
	}
	else if(FWD_EXPECTING_BWD_END_CounterErrorState== pConfig->rteErrorPatternState)
	{
		pConfig->rteErrorPatternCount++;
		if(true== pConfig->rteErrorPatternJustStarted)
		{
			pConfig->rteErrorPatternJustStarted= false;
			pConfig->rteErrorPatternCount++;
		}

		if((false== pConfig->rteErrorPatternCompensationStarted)&& (pConfig->errorPatternConfirmationCount<= pConfig->rteErrorPatternCount))
		{
			pConfig->rteErrorPatternCompensationStarted= true;
		}
		pConfig->rteErrorPatternState= BWD_EXPECTING_FWD_CounterErrorState;
	}
	pConfig->rteErrorPatternPreviousPulse= _curr;
}

static void TRACSENS_Power(bool _enable)
{
}
//===========================================================

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  * and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t pFLatency = 0;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  if (TimingDelay != 0)
  {
    TimingDelay--;
  }
  else
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    TimingDelay = LED_TOGGLE_DELAY;
  }
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    /* Reconfigure LED4 */
    BSP_LED_Init(LED4);
   /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    TimingDelay = LED_TOGGLE_DELAY;
  }
}

void UART_Printf(char *format, ...)
{
  char str[256];
  va_list args;
  va_start(args, format);
  vsnprintf(str, sizeof(str), format, args);
  va_end(args);

  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* Suspend tick */
  HAL_SuspendTick();

  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

 /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

