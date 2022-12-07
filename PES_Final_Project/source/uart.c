/**
 * @file uart.c
 * @brief UART0 driver
 *
 * This c file provides functionality for
 * initializing UART0 on the FRDM-KL25Z
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */
#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "uart.h"


#define SYS_CLOCK              (24e6)
#define UART_BAUD_RATE         (38400)
#define UART_OVERSAMPLE_RATE   (16)
#define UART_TWO_STOP_BITS     (1)
#define UART_PARITY_NONE       (0)

cbfifo_t uart_tx_cbfifo;
cbfifo_t uart_rx_cbfifo;


/**
 * @brief Writes the specified bytes to serial output
 *
 * @param handle - unused
 * @param buf    - pointer to character array to write
 * @param size   - size of character array
 *
 * @return 0 for success, -1 for failure
 */
int __sys_write(int handle, char *buf, int size) {
	// Check for validity of character buffer
	if(!buf) return -1;

	// Wait until there is enough space in the Tx circular buffer to transmit
	// the character buffer
	while((CAPACITY - cbfifo_length(&uart_tx_cbfifo)) < size)
		;

	// Enqueue the character buffer to the Tx circular buffer.
	// Return -1 if the operation fails
	if(cbfifo_enqueue(&uart_tx_cbfifo, buf, size) != size) return -1;

	// Start transmitter if it isn't already running
	if(!(UART0->C2 & UART0_C2_TIE_MASK)) {
	  UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0;
} // __sys_write()

/**
 * @brief Reads one character from the serial connection and returns it
 *
 * @return The read character. -1 if no data is available to be read
 */
int __sys_readc(void) {
	// If nothing to read, return -1
	if(cbfifo_empty(&uart_rx_cbfifo)) return -1;

	// Dequeue one character from the Rx circular buffer and return it
	uint8_t character;
	cbfifo_dequeue(&uart_rx_cbfifo, &character, sizeof(character));
	return character;
} // __sys_readc()

/**
 * @brief Initialize UART0 with interrupts
 *
 * @return none
 */
void uart0_init() {
	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;
	// Set UART clock to 24MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx
	// Set baud rate and oversampling ratio
	uint16_t sbr = (uint16_t)((SYS_CLOCK) / (UART_BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE - 1);
	// Disable interrupts for Rx active edge and LIN break detect, select two stop bits
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(UART_TWO_STOP_BITS) | UART0_BDH_LBKDIE(0);
	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(UART_PARITY_NONE);
	// Don't invert transmit data, do enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1) |
			   UART0_C3_FEIE(1) | UART0_C3_PEIE(1);
	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);
	// Initialize the Tx and Rx circular buffers
	cbfifo_init(&uart_tx_cbfifo);
	cbfifo_init(&uart_rx_cbfifo);
	// Enable interrupts
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= UART_C2_RIE(1);
	// Enable UART transmitter and receiver
	UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1);
} // uart0_init()

/**
 * @brief UART0 interrupt handler
 *
 * @return none
 */
void UART0_IRQHandler(void) {
	uint8_t character;

	if(UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK |
	   UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		// Clear error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
				     UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		// Read the data register to clear RDRF
		character = UART0->D;
	}
	if(UART0->S1 & UART0_S1_RDRF_MASK) {
		// Received a character
		character = UART0->D;
		if(!cbfifo_full(&uart_rx_cbfifo)) {
			cbfifo_enqueue(&uart_rx_cbfifo, &character, sizeof(character));
		}
		else {
			// error - queue full.
			// discard character
		}
	}
	if((UART0->C2 & UART0_C2_TIE_MASK) &&   // Transmitter interrupt enabled
	   (UART0->S1 & UART0_S1_TDRE_MASK)) {  // Tx buffer empty
		if(!cbfifo_empty(&uart_tx_cbfifo)) {
			// Can send another character
			cbfifo_dequeue(&uart_tx_cbfifo, &character, sizeof(character));
			UART0->D = character;
		}
		else {
			// uart_tx_cbfifo is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
} // UART0_IRQHandler()
