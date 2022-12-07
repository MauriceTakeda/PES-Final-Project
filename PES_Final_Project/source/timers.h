/**
 * @file timers.h
 * @brief Timer API using Systick
 *
 * This h file provides functionality for
 * tracking time
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 *
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdbool.h>


typedef uint32_t ticktime_t; // Time, in thousandths of a second

/*
 * @brief Initialize the timing system
 * @return none
 */
void       TIMER_Init();

/*
 * @brief Get time since startup in milliseconds
 * @return Time since startup in milliseconds of a second
 */
ticktime_t TIMER_Now();

/*
 * @brief Reset timer to 0; doesn't affect TIMER_Now() values
 * @return none
 */

void       TIMER_Reset();

/*
 * @brief Get time since last call to TIMER_Reset() in milliseconds
 * @return Time since last call to TIMER_Reset() in milliseconds
 */
ticktime_t TIMER_Get();

/*
 * @brief Returns whether or not the polling interval has been reached.
 *        Resets the polling timer if the interval has been reached.
 * @param period - Polling period in ms
 * @return True for polling time being reached. False otherwise.
 */
bool       TIMER_Poll(uint32_t period);

/*
 * @brief SysTick Interrupt Handler
 * @return none
 */
void       SysTick_Handler();

#endif /* TIMERS_H_ */
