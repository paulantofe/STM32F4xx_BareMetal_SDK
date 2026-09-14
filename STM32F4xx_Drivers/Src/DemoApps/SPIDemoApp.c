/**
 * @file 		SPIDemoApp.c
 * @brief 	    STM32F407xx Bare-Metal SPI driver demo
 *
 * @details		This demo uses a shift register to display digits on a 7-segment one digit display.
 *              Each button press cycles through the digits from 0 to 9.
 *
 * @note 		The hardware used in this demo is a custom PCB designed by me.
 *              TO RUN THIS DEMO: Ensure that the default main.c is excluded from build and this file is included in the project`s
 *              source path.
 *
 * Hardware Setup:
 * - Shift Register (SN74HC595N)
 * - One Digit 7-segment Display (5611AH)
 * - Push Button
 * - 220R Resistor x 7 (for each used segment)
 * - 0.1 µF Capacitor (for hybrid button debounce)
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"

__vo uint8_t digit_count = 0;
uint8_t digits[] = { 0xEE, 0x82, 0x76, 0xD6, 0x9A, 0xDC, 0xFC, 0x86, 0xFE, 0xDE }; // The digit hex codes may differ based on the cable design

__vo uint8_t tx_finished = 0;

SPI_Handle_t com = { 0 };

void SPI_Initialization(void) {
	// SPI Pins initialization
	GPIO_PClkControl(GPIOB, ENABLE);

	GPIO_Handle_t spi_pin = { 0 };
	spi_pin.pGPIOx = GPIOB;
	spi_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	spi_pin.GPIO_PinConfig.GPIO_PinAFMode = GPIO_AF5;
	spi_pin.GPIO_PinConfig.GPIO_PinOType = GPIO_OP_TYPE_PP;
	spi_pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCK PB3
	spi_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&spi_pin);

	// MOSI PB5
	spi_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GPIO_Init(&spi_pin);

	// NSS PB4
    spi_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
    spi_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
    GPIO_Init(&spi_pin);
    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_4, GPIO_PIN_SET);


    // SPI Peripheral initialization
    SPI_PClkControl(SPI1, ENABLE);

    com.pSPIx = SPI1;
    com.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    com.SPI_Config.SPI_CPHA = SPI_CPHA_FIRST_EDGE;
    com.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
    com.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD;           // Tx only is FD without MISO
    com.SPI_Config.SPI_ClkPrescaler = SPI_CLK_PRESCALER_DIV16;  // 1MHz (HSI is 16 MHz)
    com.SPI_Config.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&com);
}

void Button_Initialization(void) {
	GPIO_PClkControl(GPIOD, ENABLE);

	// Button PD7
	GPIO_Handle_t button = { 0 };
	button.pGPIOx = GPIOD;
	button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PD;
	button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;

	GPIO_Init(&button);
	GPIO_IRQInterruptConfig(EXTI9_5_IRQn, ENABLE);
}

int main(void) {
	Button_Initialization();

	SPI_Initialization();

	SPI_IRQInterruptConfig(SPI1_IRQn, ENABLE);
	SPI_SSIConfig(SPI1, ENABLE);
	SPI_PeripheralControl(SPI1, ENABLE);

	uint8_t current_digit = 0xFF; // to send digit only when button was pressed (this approach avoids sending and displaying the same digits a million times / second)
	digit_count = 0;              // to fix reset/wake-up bug (residual voltage may trigger the interrupt, falsely increasing digit_count)

	while (1) {
		if (digit_count != current_digit) {
			current_digit = digit_count;

			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_4, GPIO_PIN_RESET);

			SPI_TransmitIT(&com, digits + digit_count, 1);

			while (tx_finished == 0);
			tx_finished = 0;

			while (SPI_GetFlagStatus(SPI1, SPI_BSY_FLAG) == FLAG_SET);

			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_4, GPIO_PIN_SET);
		}
	}
}

void EXTI9_5_IRQHandler(void) {
	GPIO_IRQHandling(GPIO_PIN_NO_7);

	digit_count++;
	if (digit_count == 10) {
		digit_count = 0;
	}

	// Wait until user releases button
	while(GPIO_ReadFromInputPin(GPIOD, GPIO_PIN_NO_7) == GPIO_PIN_SET);

	// Software debounce for hybrid debounce
	for (uint32_t i = 0; i < 100000; i++);

	// Clear any false interrupt
	GPIO_IRQHandling(GPIO_PIN_NO_7);
}

void SPI1_IRQHandler(void) {
	SPI_IRQHandling(&com);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {
	if (AppEv == SPI_EVENT_TX_CMPLT) {
		tx_finished = 1;
	}
}
