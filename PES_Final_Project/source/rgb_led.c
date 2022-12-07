/**
 * @file rgb_led.c
 * @brief RGB LED driver
 *
 * This c file provides functionality for
 * driving the RGB LED
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 * @references The Dean Textbook
 *
 */
#include "MKL25Z4.h"
#include "rgb_led.h"


#define RED_LED_SHIFT                 (18)	// on port B
#define GREEN_LED_SHIFT               (19)	// on port B
#define BLUE_LED_SHIFT                (1)   // on port D

#define MAX_LED_BRIGHTNESS_LEVEL      (255) // Max led brightness level. This is the value to load into
                                            // MOD register of TPM. Chose 255 because R, G, B values can range from
                                            // 0 to 255


/*
 * @brief Initialize timer/PWM module for RGB LED.
 * @return none
 */
void RGB_LED_Init()
{
	// Enable clock to PORTB and PORTD
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	// Enable clock to TPM0 and TPM2
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;

	// Red TPM2_CH0, Mux Alt 3
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(3);
	// Green TPM2_CH1, Mux Alt 3
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(3);
	// Blue TPM0_CH1, Mux Alt 4
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(4);

	// Set clock source for TPM: 48MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	// TPM2 Setup /////////////////////////////////////////////////
	// Load the counter and mod
	TPM2->MOD = MAX_LED_BRIGHTNESS_LEVEL;
	// Set TPM count direction to up with a divide by 2 prescaler
	TPM2->SC = TPM_SC_PS(1);
	// Continue operation in debug mode
	TPM2->CONF |= TPM_CONF_DBGMODE(3);
	// Set channel 0 to edge-aligned low-true PWM
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle to zero to turn red led off
	TPM2->CONTROLS[0].CnV = 0;
	// Set channel 1 to edge-aligned low-true PWM
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle to zero to turn green led off
	TPM2->CONTROLS[1].CnV = 0;
	/////////////////////////////////////////////////////////////////

	// TPM0 Setup /////////////////////////////////////////////////
	// Load the counter and mod
	TPM0->MOD = MAX_LED_BRIGHTNESS_LEVEL;
	// Set TPM count direction to up with a divide by 2 prescaler
	TPM0->SC = TPM_SC_PS(1);
	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	// Set channel 1 to edge-aligned low-true PWM
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle to zero to turn blue led off
	TPM0->CONTROLS[1].CnV = 0;
	/////////////////////////////////////////////////////////////////

	// Start TPM2
	TPM2->SC |= TPM_SC_CMOD(1);
	// Start TPM0
	TPM0->SC |= TPM_SC_CMOD(1);
} // RGB_LED_Init()

/*
 * @brief Set RGB LED Color
 * @param r - Brightness level for red led (0-255)
 * @param g - Brightness level for green led (0-255)
 * @param b - Brightness level for blue led (0-255)
 * @return none
 */
void RGB_LED_SetColor(uint8_t r, uint8_t g, uint8_t b)
{
	// Set red led duty cycle
	TPM2->CONTROLS[0].CnV = r;
	// Set green led duty cycle
	TPM2->CONTROLS[1].CnV = g;
	// Set blue led duty cycle
	TPM0->CONTROLS[1].CnV = b;
} // RGB_LED_SetColor()
