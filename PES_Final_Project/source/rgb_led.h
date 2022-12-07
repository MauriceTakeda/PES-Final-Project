/**
 * @file rgb_led.h
 * @brief RGB LED driver
 *
 * This h file provides functionality for
 * driving the RGB LED
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */

#ifndef RGB_LED_H_
#define RGB_LED_H_

/*
 * @brief Initialize timer/PWM module for RGB LED.
 * @return none
 */
void RGB_LED_Init();

/*
 * @brief Set RGB LED Color
 * @param r - Brightness level for red led (0-255)
 * @param g - Brightness level for green led (0-255)
 * @param b - Brightness level for blue led (0-255)
 * @return none
 */
void RGB_LED_SetColor(uint8_t r, uint8_t g, uint8_t b);

#endif /* RGB_LED_H_ */
