/**
 * @file accelerometer.h
 * @brief MMA8451Q accelerometer driver
 *
 * This h file provides functionality for
 * utilizing the MMA8451Q accelerometer.
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdint.h>

/**
 * @brief Initialize the MMA8451Q accelerometer
 *
 * @return none
 */
void accelerometer_init();

/**
 * @brief Read linear acceleration from the MMA8451Q
 *
 * @return linear acceleration in units of mg
 */
float read_linear_acceleration();

#endif /* ACCELEROMETER_H_ */
