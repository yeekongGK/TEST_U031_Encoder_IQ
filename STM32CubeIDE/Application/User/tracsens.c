#include "tracsens.h"
#include "tracsens_io.h" // Include the hardware abstraction
#include <stdio.h>       // For vsnprintf
#include <string.h>      // For strlen
#include <stdarg.h>      // For va_list
#include "main.h"        // For HAL_UART_Transmit, huart2

/* ================================================================================= */
/* PRIVATE MODULE VARIABLES                               */
/* ================================================================================= */

typedef enum
{
	UNKNOWN_CounterDirection,
	FORWARD_CounterDirection,
	BACKWARD_CounterDirection,
} PULSER_CounterDirection_t;

// This variable acts as the upper 16-bits of our 32-bit counter.
static volatile int32_t lCntrMultiplier = 0;
static PULSER_CounterDirection_t eCounterDirection = UNKNOWN_CounterDirection;

// Variables for cumulative counts
static int32_t lForwardCount = 0;
static int32_t lBackwardCount = 0;
static int32_t lLastCounterValue = 0;

// UART Printf function is needed by this module but defined in main.c
// A better approach would be to have a separate logging module.


/* ================================================================================= */
/* FORWARD DECLARATIONS OF STATIC CALLBACKS                     */
/* ================================================================================= */

static void TRACSENS_AutoReloadMatchCallback(void);
static void TRACSENS_CounterChangedToUpCallback(void);
static void TRACSENS_CounterChangedToDownCallback(void);

/* ================================================================================= */
/* PUBLIC FUNCTIONS                                  */
/* ================================================================================= */

/**
 * @brief Initializes the module and its hardware dependency.
 */
int TRACSENS_Init(void)
{
    if (TRACSENS_IO_Init() != 0)
    {
        return -1; // Hardware initialization failed
    }

    // Register this module's static callbacks with the IO layer
    TRACSENS_IO_RegisterCallbacks(
        TRACSENS_AutoReloadMatchCallback,
        TRACSENS_CounterChangedToUpCallback,
        TRACSENS_CounterChangedToDownCallback
    );
    return 0;
}

/**
 * @brief Gets the full 32-bit encoder counter value.
 */
static int32_t TRACSENS_GetCounter(void)
{
    uint16_t hw_counter_val;
    int32_t multiplier_val;
    
    __disable_irq();
    hw_counter_val = TRACSENS_IO_GetCounter();
    multiplier_val = lCntrMultiplier;
    __enable_irq();

    return (multiplier_val * 65536) + hw_counter_val;
}

/**
 * @brief Starts the hardware counter.
 */
void TRACSENS_StartCounting(void)
{
    TRACSENS_IO_Start();
    // Initialize the last counter value for accurate delta calculation from the start
    lLastCounterValue = TRACSENS_GetCounter();
}

/**
 * @brief Calculates and displays sensor statistics.
 */
void TRACSENS_DisplayInfo(void)
{
	int32_t current_value = TRACSENS_GetCounter();
	int32_t delta = current_value - lLastCounterValue;
	
	if (delta > 0)
	{
		lForwardCount += delta;
	}
	else if (delta < 0)
	{
		lBackwardCount += -delta;
	}
	
	lLastCounterValue = current_value;
	
	UART_Printf("Current: %ld, Forward Total: %ld, Backward Total: %ld\r\n",
			current_value, lForwardCount, lBackwardCount);
}

/* ================================================================================= */
/* PRIVATE CALLBACK IMPLEMENTATIONS                        */
/* ================================================================================= */

/**
 * @brief  Callback for LPTIM counter direction change to UP.
 */
static void TRACSENS_CounterChangedToUpCallback(void)
{
	eCounterDirection = FORWARD_CounterDirection;
  	UART_Printf("Direction changed to UP (Forward)\r\n"); // Optional: can be noisy
}

/**
 * @brief  Callback for LPTIM counter direction change to DOWN.
 */
static void TRACSENS_CounterChangedToDownCallback(void)
{
	eCounterDirection = BACKWARD_CounterDirection;
  	UART_Printf("Direction changed to DOWN (Backward)\r\n"); // Optional: can be noisy
}

/**
 * @brief  Callback for LPTIM Auto-Reload Match (counter wraparound).
 */
static void TRACSENS_AutoReloadMatchCallback(void)
{
	int32_t current_value;
  if (eCounterDirection == FORWARD_CounterDirection)
  {
    lCntrMultiplier++;
  }
  else if (eCounterDirection == BACKWARD_CounterDirection)
  {
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
