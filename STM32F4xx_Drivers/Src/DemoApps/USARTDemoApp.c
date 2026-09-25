/**
 * @file 		USARTDemoApp.c
 * @brief 	    STM32F407xx Bare-Metal USART driver demo
 *
 * @details     This demo uses the USART Peripheral to communicate with terminal on PC
 *
 * @note        Arduino Board is used only because I do not have USART-to-USB/USB-to-TTY dedicated hardware
 *              TO RUN THIS DEMO: Ensure that the default main.c is excluded from build and this file is included in the project`s
 *              source path.
 *              Arduino Board is reseted by keeping the RESET Pin wired to GND
 *
 * Hardware Setup:
 * - Arduino Board (used for it`s USB-to-TTY converter)
 * - LCD1602 Display
 */

#include "stm32f407xx.h"
#include "stm32f407xx_usart_driver.h"

#include "lcd1602.h"

#include <string.h>

USART_Handle_t com = { 0 };
__vo uint8_t rcv_byte = 0;
__vo char    cmd_buff[10];
__vo uint8_t cmd_buff_index = 0;
__vo uint8_t first_keypress = true;
__vo uint8_t update_display = false;
__vo uint8_t col = 1;
__vo uint8_t row = 1;

void Usart_Initialization(void) {
	// USART Pins initialization
	GPIO_PClkControl(GPIOB, ENABLE);

	GPIO_Handle_t usart_pin = { 0 };
	usart_pin.pGPIOx = GPIOB;
	usart_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_pin.GPIO_PinConfig.GPIO_PinAFMode = GPIO_AF7;
	usart_pin.GPIO_PinConfig.GPIO_PinOType = GPIO_OP_TYPE_PP;
	usart_pin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	usart_pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// PB6 - USART1 Tx
	usart_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&usart_pin);

	// PB7 - USART1 Rx
	usart_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&usart_pin);

	// USART Peripheral initialization
	USART_PClkControl(USART1, ENABLE);

	com.pUSARTx = USART1;
    com.USART_Config.USART_Mode = USART_MODE_FD;
    com.USART_Config.USART_Baudrate = USART_STD_BAUD_9600;
    com.USART_Config.USART_NoOfStopBits = USART_STOP_BITS_1;
    com.USART_Config.USART_HWFlowControl = USART_HW_FLOW_NONE;
    com.USART_Config.USART_ParityControl = USART_PAR_DI;
    com.USART_Config.USART_WordLen = USART_WORD_8BITS;

    USART_Init(&com);
    USART_IRQInterruptConfig(USART1_IRQn, ENABLE);
    USART_IRQPriorityConfig(USART1_IRQn, NVIC_IRQ_PR0);
    USART_PeripheralControl(USART1, ENABLE);
    USART_SendDataIT(&com, (uint8_t*) "USART Ready. Type something...\r\n", 32);
}

void Lcd1602_Initialization(void) {
	LCD1602_Init();
	LCD1602_SendString("Type something");
	LCD1602_SetCursor(2, 1);
	LCD1602_SendString("on the TTY...");
}

void Led_Initialization(void) {
	// GPIOD Clock already enabled by lcd1602 driver
	GPIO_Handle_t led = { 0 };

	led.pGPIOx = GPIOD;
	led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	led.GPIO_PinConfig.GPIO_PinOType = GPIO_OP_TYPE_PP;
	led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;

	GPIO_Init(&led);
}

void Parse_Command(void) {
	if (strcasecmp((char*) cmd_buff, "LED ON") == 0) {
		GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_12, GPIO_PIN_SET);
		LCD1602_SendString("Led is now ON");
		first_keypress = true;
	}
	else if (strcasecmp((char*) cmd_buff, "LED OFF") == 0) {
		GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_12, GPIO_PIN_RESET);
		LCD1602_SendString("Led is now OFF");
	    first_keypress = true;
	}
	else {
		LCD1602_SendString("Unknown command.");
		LCD1602_SetCursor(2, 1);
		LCD1602_SendString("Try again.");
		first_keypress = true;
	}
}

void Print_Char(void) {
	LCD1602_SetCursor(row, col);
	LCD1602_SendChar(rcv_byte);

	if (cmd_buff_index < 9) {
		cmd_buff[cmd_buff_index++] = rcv_byte;
	}

	col++;
	if (col > 16) {
	    row++;
	   	col = 1;
	}
	if (row > 2) {
	    row = 1;
	}
}

void Special_Char(uint8_t code) {
	if (code == 8) {
		// Backspace
		col--;
		if (col < 1) {
			if (row > 1) {
				row--;
				col = 16;
			}
			else {
				col = 1;
			}
		}

		LCD1602_SetCursor(row, col);
		LCD1602_SendChar(' ');

		if (cmd_buff_index > 0) {
			cmd_buff_index--;
		}
	}
	else if (code == 13) {
		// Carriage Return (interpreted as ENTER command)
		LCD1602_SendCommand(LCD1602_CMD_CLEAR);
		row = 1;
		col = 1;
		LCD1602_SetCursor(row, col);

		cmd_buff[cmd_buff_index] = '\0';

		Parse_Command();

		cmd_buff_index = 0;
	}
}

int main(void) {
	Usart_Initialization();
	Lcd1602_Initialization();
	Led_Initialization();

	USART_ReceiveDataIT(&com, (uint8_t*) &rcv_byte, 1);

    while (true) {
    	if (update_display) {
    		update_display = false;

    		// Drop Line Feed
    		if (rcv_byte == 10) {
    		    continue;
    		}

    		if (first_keypress) {
    			first_keypress = false;
    			LCD1602_SendCommand(LCD1602_CMD_CLEAR);
    			row = 1;
    			col = 1;
    		}

    		if (rcv_byte == 8 || rcv_byte == 13) {
    			// Special character (Backspace, Carriage Return)
    			Special_Char(rcv_byte);
    		}
    		else {
    			// Normal character print
    			Print_Char();
    		}
    	}
    }

	return 0;
}

void USART1_IRQHandler(void) {
	USART_IRQHandling(&com);
}

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv) {
	if (AppEv == USART_EVENT_RX_CMPLT) {
		update_display = true;

		USART_ReceiveDataIT(&com, (uint8_t*) &rcv_byte, 1);
	}
}
