/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    PES_Final_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdint.h>
#include "sysclock.h"
#include "timers.h"
#include "rgb_led.h"
#include "cbfifo.h"
#include "uart.h"
#include "cmd_processor.h"
#include "i2c.h"
#include "accelerometer.h"


uint8_t target_r_val        = 0;     // RGB LED r value to set when detected acceleration reaches target
uint8_t target_g_val        = 255;   // RGB LED g value to set when detected acceleration reaches target
uint8_t target_b_val        = 0;     // RGB LED b value to set when detected acceleration reaches target
float   target_acceleration = 10;    // Target acceleration value in m/s^2
bool    print_acceleration  = false; // True means print acceleration values, False means don't print acceleration values


int main(void) {
  // Initialize peripherals
  sysclock_init();
  TIMER_Init();
  RGB_LED_Init();
  RGB_LED_SetColor(255, 255, 255);
  uart0_init();
  i2c_init();
  accelerometer_init();

#if DEBUG
  // Test circular buffer API
  cbfifo_test();
#endif

  // Print application introduction message
  printf("\n\r");
  printf("------------------------------------------------\n\r");
  printf("Acceleration Detector Command Terminal\n\r");
  printf("------------------------------------------------\n\r");
  printf("GENERAL INFO\n\r");
  printf("Place the FRDM-KL25Z flat on a surface. Move the board while keeping it flat.\n\r");
  printf("If you reach the target acceleration, then the RGB LED will change colors!\n\r");
  printf("Be sure to keep the board flat, and not rotated, otherwise the acceleration due to\n\r");
  printf("gravity will negatively affect the acceleration measurements.\n\r");
  printf("COMMAND INFO\n\r");
  printf("Command to set target color         : color <r> <g> <b>\n\r");
  printf("Command to set target acceleration  : acceleration <target acceleration>\n\r");
  printf("Command to print acceleration values: print\n\r");
  printf("DEFAULT VALUES\n\r");
  printf("Default target color r=%d, g=%d, b=%d\n\r", target_r_val, target_g_val, target_b_val);
  printf("Default target acceleration = %f m/s^2\n\r", target_acceleration);
  printf("------------------------------------------------\n\r");
  printf("\n\r");
  printf("> ");

  double acceleration = 0.0;
  TIMER_Reset();
  // Infinite loop
  while (1) {

    // Blocking receive
	while(cbfifo_empty(&uart_rx_cbfifo)) {
		// Read acceleration values and convert from mg to m/s^2
		acceleration = (read_linear_acceleration() * 9.80665) / 1000;
		// Print acceleration value in 1s intervals if printing is enabled
		if(TIMER_Get() >= 1000 && print_acceleration) {
			printf("acceleration = %f m/s^2\n\r", acceleration);
			TIMER_Reset();
		}
		// Update RGB LED color based on acceleration measurement
		if(acceleration >= target_acceleration) {
			RGB_LED_SetColor(target_r_val, target_g_val, target_b_val);
		}
		else {
			RGB_LED_SetColor(255, 255, 255);
		}
	}

	// Accumulate received characters
	accumulate_line();
  }

  return 0 ;
}
