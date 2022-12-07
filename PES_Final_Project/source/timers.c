/**
 * @file timers.c
 * @brief Timer API using Systick
 *
 * This c file provides functionality for
 * tracking time
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 *
 */
#include "MKL25Z4.h"
#include "timers.h"


static ticktime_t time_get        = 0; // Time since last call to TIMER_Reset() in thousandths of a second
static ticktime_t time_now        = 0; // Time since boot in thousandths of a second
static ticktime_t time_poll       = 0; // Time since last touch sense polling in thousandths of a second

/*
 * @brief Initialize the timing system
 * @return none
 */
void TIMER_Init()
{
	SysTick->LOAD = 3000 - 1; // Set reload to get 1ms interrupts
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | // Enable interrupts
			        SysTick_CTRL_ENABLE_Msk;   // Enable counter
} // TIMER_Init()


/*
 * @brief Get time since startup in milliseconds
 * @return Time since startup in milliseconds of a second
 */
ticktime_t TIMER_Now()
{
	return time_now;
} // TIMER_Now()


/*
 * @brief Reset timer to 0; doesn't affect TIMER_Now() values
 * @return none
 */
void TIMER_Reset()
{
	time_get = 0;
} // TIMER_Reset()


/*
 * @brief Get time since last call to TIMER_Reset() in milliseconds
 * @return Time since last call to TIMER_Reset() in milliseconds
 */
ticktime_t TIMER_Get()
{
	return time_get;
} // TIMER_Get()

/*
 * @brief Returns whether or not the polling interval has been reached.
 *        Resets the polling timer if the interval has been reached.
 * @param period - Polling period in ms
 * @return True for polling time being reached. False otherwise.
 */
bool TIMER_Poll(uint32_t period)
{
	if(time_poll >= period) {
		time_poll = 0;
		return true;
	}

	return false;
} // TIMER_Poll()

/*
 * @brief SysTick Interrupt Handler
 * @return none
 */
void SysTick_Handler()
{
	time_get++;
	time_now++;
	time_poll++;
} // SysTick_Handler()

