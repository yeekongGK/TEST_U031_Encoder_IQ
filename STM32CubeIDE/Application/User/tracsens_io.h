#ifndef TRACSENS_IO_H
#define TRACSENS_IO_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Enumeration for hardware counter direction.
 */
typedef enum
{
    TRACSENS_IO_DIR_UP,
    TRACSENS_IO_DIR_DOWN
} TRACSENS_IO_Direction_t;

/**
 * @brief Initializes the underlying hardware timer/counter for the tracking sensor.
 * @retval 0 on success, non-zero on failure.
 */
int TRACSENS_IO_Init(void);

/**
 * @brief Starts the hardware counter.
 */
void TRACSENS_IO_Start(void);

/**
 * @brief Gets the current raw value from the hardware counter.
 * @retval The 16-bit counter value.
 */
uint16_t TRACSENS_IO_GetCounter(void);

/**
 * @brief Registers the application-level callback functions with the hardware driver.
 * @param reload_cb   Function pointer for the auto-reload (overflow/underflow) event.
 * @param dir_up_cb   Function pointer for the direction change to 'up' event.
 * @param dir_down_cb Function pointer for the direction change to 'down' event.
 */
void TRACSENS_IO_RegisterCallbacks(void (*reload_cb)(void), void (*dir_up_cb)(void), void (*dir_down_cb)(void));

/**
 * @brief Checks if a hardware counter wraparound (overflow/underflow) is pending.
 * @retval true if the Auto-Reload Match flag is set, false otherwise.
 */
bool TRACSENS_IO_IsWrapAroundPending(void);

/**
 * @brief Gets the current counting direction from the hardware.
 * @retval The current direction (UP or DOWN).
 */
TRACSENS_IO_Direction_t TRACSENS_IO_GetDirection(void);


LPTIM_HandleTypeDef* TRACSENS_IO_GetHandle(void);

#endif // TRACSENS_IO_H

