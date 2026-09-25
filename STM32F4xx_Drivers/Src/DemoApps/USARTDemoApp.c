/**
 * @file 		USARTDemoApp.c
 * @brief 	    STM32F407xx Bare-Metal USART driver demo
 *
 * @details
 *
 * @note        TO RUN THIS DEMO: Ensure that the default main.c is excluded from build and this file is included in the project`s
 *              source path.
 *
 * Hardware Setup:
 * - Arduino Board
 * - LCD1602 Display
 */

#include "stm32f407xx.h"
#include "stm32f407xx_usart_driver.h"

#include "lcd1602.h"

#include <string.h>

#define true      1
#define false     0

USART_Handle_t com = { 0 };

char buffer[32];
uint32_t buff_index = 0;

uint8_t rcv_byte = 0;
volatile uint8_t col = 1; // Assuming your LCD columns are 1-16
volatile uint8_t update = false;

void delay(void) {
	for (__vo uint32_t i = 0; i < 5000000; i++);
}

void USART_Initialization(void) {
	GPIO_PClkControl(GPIOB, ENABLE);

	GPIO_Handle_t usart_pin = { 0 };
	usart_pin.pGPIOx = GPIOB;
	usart_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_pin.GPIO_PinConfig.GPIO_PinAFMode = GPIO_AF7;
	usart_pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// USART1-Tx PB6
	usart_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&usart_pin);

	// USART1-Rx PB7
	usart_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&usart_pin);

	USART_PClkControl(USART1, ENABLE);

	com.pUSARTx = USART1;
	com.USART_Config.USART_Mode = USART_MODE_FD;
	com.USART_Config.USART_Baudrate = USART_STD_BAUD_9600;
	com.USART_Config.USART_HWFlowControl = USART_HW_FLOW_NONE;
	com.USART_Config.USART_NoOfStopBits = USART_STOP_BITS_1;
	com.USART_Config.USART_ParityControl = USART_PAR_DI;
	com.USART_Config.USART_WordLen = USART_WORD_8BITS;

	USART_Init(&com);
	USART_IRQInterruptConfig(USART1_IRQn, ENABLE);
	USART_PeripheralControl(USART1, ENABLE);

	LCD1602_SendString("USART Ready");
	USART_SendDataIT(&com, (uint8_t*)"USART Initialized\n\r", 19);

	delay();
	LCD1602_SendCommand(LCD1602_CMD_CLEAR);
}

int main(void) {
	LCD1602_Init();
	USART_Initialization();

	USART_ReceiveDataIT(&com, &rcv_byte, 1);

    while (true) {
    	if (update) {
    		update = false;
    		LCD1602_SetCursor(1, col);
    		LCD1602_SendChar(rcv_byte);

    		col++;
    		if (col > 16) col = 1;
    	}
    }

	return 0;
}

void USART1_IRQHandler(void) {
    USART_IRQHandling(&com);
}

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv) {
	if (AppEv == USART_EVENT_RX_CMPLT) {
	    update = true; // Just set the flag, let main loop handle the cursor math
	    USART_ReceiveDataIT(&com, (uint8_t*)&rcv_byte, 1);
	}
}
