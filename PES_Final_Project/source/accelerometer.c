/**
 * @file accelerometer.c
 * @brief MMA8451Q accelerometer driver
 *
 * This c file provides functionality for
 * utilizing the MMA8451Q accelerometer.
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */
#include <math.h>
#include "i2c.h"
#include "accelerometer.h"


#define MMA_ADDR  0x3A // I2C address for MMA8451Q accelerometer
#define REG_CTRL1 0x2A // CTRL1 register address for MMA8451Q
#define REG_XHI   0x01 // X_OUT_MSB register address for MMA8451Q


/**
 * @brief Initialize the MMA8451Q accelerometer
 *
 * @return none
 */
void accelerometer_init() {
	// Set active mode, 14 bit samples, and 800Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
} // accelerometer_init()

/**
 * @brief Read linear acceleration from the MMA8451Q
 *
 * @return linear acceleration in units of mg
 */
float read_linear_acceleration() {
	uint8_t data[4];
	int16_t acc_x_raw = 0, acc_y_raw = 0;
	int16_t acc_x = 0, acc_y = 0;
	float linear_acc;

	i2c_read_bytes(MMA_ADDR, REG_XHI, data, sizeof(data));
	acc_x_raw = (int16_t)((data[0] << 8) | data[1]);
	acc_y_raw = (int16_t)((data[2] << 8) | data[3]);

	// Align for 14 bits
	acc_x = acc_x_raw >> 2;
	acc_y = acc_y_raw >> 2;
	// Range is -2g to 2g so need to divide by 4 in order to get value in mg according to datasheet
	acc_x /= 4;
	acc_y /= 4;
	// Calculate linear acceleration based on x-axis and y-axis accelerations
	linear_acc = sqrt((acc_x * acc_x) + (acc_y * acc_y));

	return linear_acc;
} // read_linear_acceleration()
