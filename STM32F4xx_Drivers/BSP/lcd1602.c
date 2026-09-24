/**
 * @file 		lcd1602.c
 * @brief 		Source file for the LCD1602 display driver.
 *
 * @details
 */

#include "lcd1602.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */

static void delay_ms(uint32_t ms) {
	for (__vo uint32_t i = 0; i < (ms * 1000); i++);
}

static void delay_us(uint32_t us) {
	for (__vo uint32_t i = 0; i < us; i++);
}

static void lcd1602_enable(void) {
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_E , GPIO_PIN_SET);

	delay_us(10);

	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_E , GPIO_PIN_RESET);

	delay_us(100);
}

static void lcd1602_gpio_pins_init(void) {
	GPIO_PClkControl(LCD1602_GPIO_PORT, ENABLE);

	GPIO_Handle_t lcd_pin = { 0 };
	lcd_pin.pGPIOx = LCD1602_GPIO_PORT;
	lcd_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	lcd_pin.GPIO_PinConfig.GPIO_PinOType = GPIO_OP_TYPE_PP;
	lcd_pin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// RS Pin Initialization
	lcd_pin.GPIO_PinConfig.GPIO_PinNumber = LCD1602_GPIO_RS;
	GPIO_Init(&lcd_pin);

	// E Pin Initialization
	lcd_pin.GPIO_PinConfig.GPIO_PinNumber = LCD1602_GPIO_E;
	GPIO_Init(&lcd_pin);

	// D4 Pin Initialization
	lcd_pin.GPIO_PinConfig.GPIO_PinNumber = LCD1602_GPIO_D4;
	GPIO_Init(&lcd_pin);

	// D5 Pin Initialization
	lcd_pin.GPIO_PinConfig.GPIO_PinNumber = LCD1602_GPIO_D5;
	GPIO_Init(&lcd_pin);

	// D6 Pin Initialization
	lcd_pin.GPIO_PinConfig.GPIO_PinNumber = LCD1602_GPIO_D6;
	GPIO_Init(&lcd_pin);

	// D7 Pin Initialization
	lcd_pin.GPIO_PinConfig.GPIO_PinNumber = LCD1602_GPIO_D7;
	GPIO_Init(&lcd_pin);

	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_E , GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D7, GPIO_PIN_RESET);
}

static void lcd1602_write_to_data_pins(uint8_t hex_value) {
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D4, ((hex_value >> 0) & 0x1));
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D5, ((hex_value >> 1) & 0x1));
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D6, ((hex_value >> 2) & 0x1));
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_D7, ((hex_value >> 3) & 0x1));

	lcd1602_enable();
}

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Initialize LCD1602 display
 * @retval None
 */
void LCD1602_Init(void) {
	lcd1602_gpio_pins_init();

	// Hardware Wake-up and 4-bit mode lock
    delay_ms(40);
    lcd1602_write_to_data_pins(0x3);
	delay_ms(5);
    lcd1602_write_to_data_pins(0x3);
    delay_us(150);
    lcd1602_write_to_data_pins(0x3);
    lcd1602_write_to_data_pins(0x2);

    // Display Configuration
    // Set 4-bit mode, 2 lines, 5x8 font
    LCD1602_SendCommand(LCD1602_CMD_FUNC_4BIT_2LINES);

    // Turn display ON, Cursor OFF
    LCD1602_SendCommand(LCD1602_CMD_DISP_ON_CUR_OFF);

    // Clear the display RAM
    LCD1602_SendCommand(LCD1602_CMD_CLEAR);

    // Set entry mode (auto-increment cursor left-to-right)
    LCD1602_SendCommand(LCD1602_CMD_ENTRY_DIR_RIGHT);
}

/**
 * @brief  Send a command to LCD1602 display
 * @retval None
 */
void LCD1602_SendCommand(uint8_t cmd) {
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_RS, GPIO_PIN_RESET);

	// Send higher nibble of the command
	lcd1602_write_to_data_pins(cmd >> 4);

	// Send lower nibble of the command
	lcd1602_write_to_data_pins(cmd & 0x0F);

	// Extra time for slow screen commands
	if (cmd == LCD1602_CMD_CLEAR || cmd == LCD1602_CMD_RET_HOME) {
		delay_ms(2);
	}
}

/**
 * @brief  Send a character to LCD1602 display
 * @retval None
 */
void LCD1602_SendChar(uint8_t ch) {
	GPIO_WriteToOutputPin(LCD1602_GPIO_PORT, LCD1602_GPIO_RS, GPIO_PIN_SET);

	// Send higher nibble of the character
	lcd1602_write_to_data_pins(data >> 4);

	// Send lower nibble of the character
	lcd1602_write_to_data_pins(data & 0x0F);
}

/**
 * @brief  Send a string to LCD1602 display
 * @retval None
 */
void LCD1602_SendString(char *str) {
	while (*str) {
		LCD1602_SendChar((uint8_t) *str++);
	}
}

/**
 * @brief  Set cursor on LCD1602 display
 * @retval None
 */
void LCD1602_SetCursor(uint8_t row, uint8_t col) {
	col--;

	switch (row) {
	case 1:
		LCD1602_SendCommand(col |= 0x80);
		break;

	case 2:
		LCD1602_SendCommand(col |= 0xC0);
		break;

	default:
		break;
	}
}


/* ----------------------------------------------------------------------------------- */
