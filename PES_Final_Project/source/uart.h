/**
 * @file uart.h
 * @brief UART0 driver
 *
 * This h file provides functionality for
 * initializing UART0 on the FRDM-KL25Z
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */
#ifndef UART_H_
#define UART_H_

#include "cbfifo.h"

extern cbfifo_t uart_tx_cbfifo;
extern cbfifo_t uart_rx_cbfifo;

/**
 * @brief Initialize UART0
 *
 * @return none
 */
void uart0_init();

#endif /* UART_H_ */
