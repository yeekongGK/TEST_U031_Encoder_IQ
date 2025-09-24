#ifndef TRACSENS_H
#define TRACSENS_H

/**
 * @brief Initializes the Tracking Sensor module and its underlying hardware.
 * @retval 0 on success, non-zero on failure.
 */
int TRACSENS_Init(void);

/**
 * @brief Starts the sensor counting process.
 */
void TRACSENS_StartCounting(void);

/**
 * @brief Calculates and displays sensor statistics via UART.
 * @note  This function updates the cumulative forward/backward counts.
 */
void TRACSENS_DisplayInfo(void);

#endif // TRACSENS_H
