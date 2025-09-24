#include "tracsens_io.h"
#include "main.h" // For STM32 HAL/LL drivers

// LPTIM handle is now private to this hardware-specific file
static LPTIM_HandleTypeDef hlptim1;

// Pointers to the application-level callbacks
static void (*App_ReloadCallback)(void) = NULL;
static void (*App_DirUpCallback)(void) = NULL;
static void (*App_DirDownCallback)(void) = NULL;

/* STM32-specific HAL Callbacks that will invoke the application callbacks */
static void LPTIM1_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim);
static void LPTIM1_CounterChangedToUpCallback(LPTIM_HandleTypeDef *hlptim);
static void LPTIM1_CounterChangedToDownCallback(LPTIM_HandleTypeDef *hlptim);


LPTIM_HandleTypeDef* TRACSENS_IO_GetHandle(void)
{
    return &hlptim1;
}

/**
 * @brief Initializes the LPTIM1 peripheral in Encoder mode.
 */
int TRACSENS_IO_Init(void)
{
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.UltraLowPowerClock.Polarity = LPTIM_CLOCKPOLARITY_RISING;
  hlptim1.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.Period = 0xFFFF; // Autoreload value
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_EXTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim1.Init.RepetitionCounter = 0;

  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    return -1; // Error
  }
  return 0; // Success
}

/**
 * @brief Starts the LPTIM1 counter.
 */
void TRACSENS_IO_Start(void)
{
    if (HAL_LPTIM_Counter_Start_IT(&hlptim1) != HAL_OK)
    {
        // In a real application, you would handle this error
        while(1);
    }
    
	/* Enable the required interrupts */
	LL_LPTIM_EnableIT_ARRM(LPTIM1); 	
	LL_LPTIM_EnableIT_UP(LPTIM1);		
	LL_LPTIM_EnableIT_DOWN(LPTIM1);

	LL_LPTIM_SetEncoderMode(LPTIM1, LL_LPTIM_ENCODER_MODE_RISING_FALLING);
    LL_LPTIM_EnableEncoderMode(LPTIM1);
    LL_LPTIM_Enable(LPTIM1);
	LL_LPTIM_SetAutoReload(LPTIM1, 0xFFFF);
    LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS);
}

/**
 * @brief Reads the LPTIM1 counter register.
 */
uint16_t TRACSENS_IO_GetCounter(void)
{
    return LL_LPTIM_GetCounter(LPTIM1);
}

/**
 * @brief Registers application callbacks and assigns them to the HAL LPTIM callbacks.
 */
void TRACSENS_IO_RegisterCallbacks(void (*reload_cb)(void), void (*dir_up_cb)(void), void (*dir_down_cb)(void))
{
    // Store the application-level function pointers
    App_ReloadCallback = reload_cb;
    App_DirUpCallback = dir_up_cb;
    App_DirDownCallback = dir_down_cb;

    // Register the local, hardware-specific ISR handlers with the HAL driver
    HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_AUTORELOAD_MATCH_CB_ID, LPTIM1_AutoReloadMatchCallback);
	HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_DIRECTION_UP_CB_ID, LPTIM1_CounterChangedToUpCallback);
	HAL_LPTIM_RegisterCallback(&hlptim1, HAL_LPTIM_DIRECTION_DOWN_CB_ID, LPTIM1_CounterChangedToDownCallback);
}


/* ----------------- STM32 HAL-Specific Callback Implementations ----------------- */

/**
 * @brief  LPTIM1 Auto-Reload Match callback.
 * @note   This function is called by the HAL driver on a counter wraparound.
 * It then calls the registered application-level callback.
 */
static void LPTIM1_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    if (App_ReloadCallback != NULL)
    {
        App_ReloadCallback();
    }
}

/**
 * @brief  LPTIM1 Counter direction change to UP callback.
 */
static void LPTIM1_CounterChangedToUpCallback(LPTIM_HandleTypeDef *hlptim)
{
    if (App_DirUpCallback != NULL)
    {
        App_DirUpCallback();
    }
}

/**
 * @brief  LPTIM1 Counter direction change to DOWN callback.
 */
static void LPTIM1_CounterChangedToDownCallback(LPTIM_HandleTypeDef *hlptim)
{
    if (App_DirDownCallback != NULL)
    {
        App_DirDownCallback();
    }
}
