/**
 * @file 		lcd1602.h
 * @brief 		Board Support Package (BSP) driver header for the LCD1602 display.
 *
 * @details
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

/* -------------------------- GENERIC MACROS -------------------------- */

/**
 * @note     RW pin is always grounded
 * @defgroup LCD1602_APP_CFG   Application Configurable Items
 * @{
 */
#define LCD1602_GPIO_PORT      GPIOD
#define LCD1602_GPIO_RS        GPIO_PIN_NO_0
#define LCD1602_GPIO_E         GPIO_PIN_NO_2
#define LCD1602_GPIO_D4        GPIO_PIN_NO_3
#define LCD1602_GPIO_D5        GPIO_PIN_NO_4
#define LCD1602_GPIO_D6        GPIO_PIN_NO_5
#define LCD1602_GPIO_D7        GPIO_PIN_NO_6
/** @} */

/**
 * @defgroup LCD1602_CMDS               Screen Commands
 * @{
 */
#define LCD1602_CMD_CLEAR               0x01
#define LCD1602_CMD_RET_HOME            0x02

// Entry Mode Set
#define LCD1602_CMD_ENTRY_DIR_LEFT      0x04  // Decrement cursor (Right-to-Left)
#define LCD1602_CMD_ENTRY_DIR_RIGHT     0x06  // Increment cursor (Left-to-Right - Standard)
#define LCD1602_CMD_ENTRY_SHIFT_EN      0x07  // Increment cursor and shift display

// Display ON/OFF Control
#define LCD1602_CMD_DISP_OFF            0x08  // Display off, cursor off, blink off
#define LCD1602_CMD_DISP_ON_CUR_OFF     0x0C  // Display on, cursor off
#define LCD1602_CMD_DISP_ON_CUR_ON      0x0E  // Display on, cursor on, solid
#define LCD1602_CMD_DISP_ON_CUR_BLINK   0x0F  // Display on, cursor blinking

// Cursor/Display Shift
#define LCD1602_CMD_SHIFT_CUR_LEFT      0x10  // Move cursor left by 1
#define LCD1602_CMD_SHIFT_CUR_RIGHT     0x14  // Move cursor right by 1
#define LCD1602_CMD_SHIFT_DISP_LEFT     0x18  // Shift entire display left by 1
#define LCD1602_CMD_SHIFT_DISP_RIGHT    0x1C  // Shift entire display right by 1

// Function Set (4-bit mode specific)
#define LCD1602_CMD_FUNC_4BIT_1LINE     0x20  // 4-bit data, 1 line, 5x8 font
#define LCD1602_CMD_FUNC_4BIT_2LINES    0x28  // 4-bit data, 2 lines, 5x8 font

// Memory Address Base Commands
#define LCD1602_CMD_CGRAM_ADDR          0x40  // Base command for Custom Character RAM
#define LCD1602_CMD_DDRAM_ADDR          0x80  // Base command for Display Data RAM
/** @} */

/* -------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

void LCD1602_Init(void);

void LCD1602_SendCommand(uint8_t cmd);

void LCD1602_SendChar(uint8_t ch);

void LCD1602_SendString(char *str);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
