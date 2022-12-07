/**
 * @file i2c.h
 * @brief I2C driver
 *
 * This h file provides functionality for
 * utilizing i2c
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

/**
 * @brief Initialize the I2C0 peripheral
 *
 * @return none
 */
void i2c_init();

/**
 * @brief Write byte of data using i2c
 *
 * @param dev  - Device address to write to
 * @param reg  - Register address to write to
 * @param data - Byte of data to write
 *
 * @return none
 */
void i2c_write_byte(uint8_t dev, uint8_t reg, uint8_t data);

/**
 * @brief Read bytes of data using i2c
 *
 * @param dev        - Device address to read from
 * @param reg        - Register address to read from
 * @param data       - Pointer to where read data will be stored
 * @param data_count - Number of bytes to read
 *
 * @return The bytes of data that was read from i2c bus
 */
void i2c_read_bytes(uint8_t dev, uint8_t reg, uint8_t * data, int8_t data_count);

#endif /* I2C_H_ */
