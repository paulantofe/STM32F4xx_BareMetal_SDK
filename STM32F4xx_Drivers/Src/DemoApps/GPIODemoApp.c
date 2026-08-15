/**
 * @file 		GPIODemoApp.c
 * @brief 	    STM32F407xx Bare-Metal GPIO driver demo
 *
 * @details		This demo uses a software PWM to control the brightness of the LEDs.
 *              Each button press increases the duty cycle in steps of 25% (from 0% - OFF to 100% - FULL BRIGHTNESS).
 *              A secret LED pattern can be unlocked with 4 fast consecutive button presses.
 *
 * @note 		This demo does not use hardware TIMERS as, for the time being, they are not yet supported by the custom driver layer.
 *              TO RUN THIS DEMO: Ensure that the default main.c is excluded from build and this file is included in the project`s
 *              source path.
 *
 * Hardware Setup (On-board components):
 * - Green LED   : PD12
 * - Orange LED  : PD13
 * - Red LED     : PD14
 * - Blue LED    : PD15
 * - User Button : PA0
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

/* -------------------------- PARAMETER CONFIGURATION -------------------------- */

#define DEBOUNCE_DELAY         15000
#define PATTERN_DELAY          50000
#define TIMEOUT_THRESHOLD      100
#define PWM_STEP_MULTIPLIER    10

//* ---------------------------------------------------------------------------- */


/* -------------------------- GLOBAL VARIABLES -------------------------- */

static uint8_t ledPins[] = { GPIO_PIN_NO_12, GPIO_PIN_NO_13, GPIO_PIN_NO_14, GPIO_PIN_NO_15 };
static uint8_t buttonPin = GPIO_PIN_NO_0;

static __vo uint8_t dutyCycle = 0;

static __vo uint8_t secretCounter = 0;
static __vo uint32_t inactivityTimer = 0;
static __vo uint8_t secretModeActive = 0;

/* ---------------------------------------------------------------------- */


/* -------------------------- INITIALIZATION FUNCTIONS -------------------------- */

void ledsInit(void) {
	GPIO_PCLKControl(GPIOD, ENABLE);

	GPIO_Handle_t led = { 0 };
	led.pGPIOx = GPIOD;
	led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	led.GPIO_PinConfig.GPIO_PinOType = GPIO_OP_TYPE_PP;

	for (uint8_t i = 0; i < 4; i++) {
		led.GPIO_PinConfig.GPIO_PinNumber = ledPins[i];
		GPIO_Init(&led);
	}
}

void buttonInit(void) {
	GPIO_PCLKControl(GPIOA, ENABLE);

	GPIO_Handle_t button = { 0 };
	button.pGPIOx = GPIOA;
	button.GPIO_PinConfig.GPIO_PinNumber = buttonPin;
	button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;

	GPIO_Init(&button);
	GPIO_IRQInterruptConfig(EXTI0_IRQn, ENABLE);
	GPIO_IRQPriorityConfig(EXTI0_IRQn, NVIC_IRQ_PR0);
}

/* ------------------------------------------------------------------------------ */


/* -------------------------- MAIN APP -------------------------- */

int main(void) {
	ledsInit();
	buttonInit();

	while (1) {
		// Timeout for secret pattern
		if (secretCounter > 0) {
			inactivityTimer++;
			if (inactivityTimer > TIMEOUT_THRESHOLD) {
				inactivityTimer = 0;
				secretCounter = 0;
			}
		}

		// Secret mode (LED animation)
		if (secretModeActive) {
			for (uint8_t repeat = 0; repeat < 3; repeat++) {
				for (uint8_t i = 0; i < 4; i++) {
					GPIO_WriteToOutputPin(GPIOD, ledPins[i], GPIO_PIN_SET);
					for (__vo uint32_t j = 0; j < PATTERN_DELAY; j++);
					GPIO_WriteToOutputPin(GPIOD, ledPins[i], GPIO_PIN_RESET);
			    }
			}
			secretModeActive = 0;
		}
		// Normal mode (Software PWM)
		else {
			if (dutyCycle > 0) {
				for (uint8_t i = 0; i < 4; i++) {
					GPIO_WriteToOutputPin(GPIOD, ledPins[i], GPIO_PIN_SET);
				}
				for (__vo uint32_t i = 0; i < dutyCycle * PWM_STEP_MULTIPLIER; i++);
			}

			if (dutyCycle < 100) {
				for (uint8_t i = 0; i < 4; i++) {
					GPIO_WriteToOutputPin(GPIOD, ledPins[i], GPIO_PIN_RESET);
				}
				for (__vo uint32_t i = 0; i < (100 - dutyCycle) * PWM_STEP_MULTIPLIER; i++);
			}
		}
	}
}

/* -------------------------------------------------------------- */


/* -------------------------- INTERRUPT SERVICE ROUTINE FOR EXTI0 -------------------------- */

void EXTI0_IRQHandler(void) {
	GPIO_IRQHandling(GPIO_PIN_NO_0);

	if (secretModeActive) {
		return;
	}

	for (volatile uint32_t i = 0; i < DEBOUNCE_DELAY; i++);

	if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)) {
		dutyCycle += 25;
		if (dutyCycle > 100) {
			dutyCycle = 0;
		}
		secretCounter++;
		inactivityTimer = 0;
		if (secretCounter == 4) {
			secretModeActive = 1;
			secretCounter = 0;
		}
	}
}

/* ----------------------------------------------------------------------------------------- */

