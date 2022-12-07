/**
 * @file i2c.c
 * @brief I2C driver
 *
 * This c file provides functionality for
 * utilizing i2c.
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */
#include "MKL25Z4.h"
#include "i2c.h"


#define I2C_M_START     I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP      I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART    I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_TRAN        I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC         I2C0->C1 &= ~I2C_C1_TX_MASK
#define I2C_WAIT        while((I2C0->S & I2C_S_IICIF_MASK)==0) {} \
	                    	I2C0->S |= I2C_S_IICIF_MASK;
#define NACK            I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK             I2C0->C1 &= ~I2C_C1_TXAK_MASK


/**
 * @brief Initialize the I2C0 peripheral
 *
 * @return none
 */
void i2c_init() {
	// Clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	// Set pins to i2c function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);
	// Set to 400k baud
	// Baud = bus freq / (scl_div + mul)
	// 24MHz/400kHz = 60; icr=0x11 sets scl_div to 56
	I2C0->F = I2C_F_ICR(0x11) | I2C_F_MULT(0);
	// Enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);
	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
} // i2c_init()

/**
 * @brief Write byte of data using i2c
 *
 * @param dev  - Device address to write to
 * @param reg  - Register address to write to
 * @param data - Byte of data to write
 *
 * @return none
 */
void i2c_write_byte(uint8_t dev, uint8_t reg, uint8_t data) {
	// Set to transmit mode
	I2C_TRAN;
	// Send start
	I2C_M_START;
	// Send device address (write)
	I2C0->D = dev;
	// Wait for ACK
	I2C_WAIT
	// Send register address
	I2C0->D = reg;
	I2C_WAIT
	// Send data
	I2C0->D = data;
	I2C_WAIT
	I2C_M_STOP;
} // i2c_write_byte()

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
void i2c_read_bytes(uint8_t dev, uint8_t reg, uint8_t * data, int8_t data_count) {
	uint8_t dummy;
	int8_t num_bytes_read = 0;
	I2C_TRAN; // Set to transmit mode
	I2C_M_START; // Send start
	I2C0->D = dev; // Send dev address (write)
	I2C_WAIT // Wait for completion
	I2C0->D = reg; // Send register address
	I2C_WAIT // Wait for completion
	I2C_M_RSTART; // Repeated start
	I2C0->D = (dev | 0x1); // Send device address (read)
	I2C_WAIT // Wait for completion
	I2C_REC; // Set to receive mode
	ACK; // Set ACK after read
	dummy = I2C0->D; // Dummy read
	I2C_WAIT // Wait for completion
	do {
		ACK; // Set ACK after read
		data[num_bytes_read++] = I2C0->D; // Read data
		I2C_WAIT // Wait for completion
	} while(num_bytes_read < data_count - 2);
	NACK; // Set NACK after read
	data[num_bytes_read++] = I2C0->D; // Read data
	I2C_WAIT // Wait for completion
	I2C_M_STOP; // Send stop
} // i2c_read_byte()
