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
#define LCD1602_GPIO_EN        GPIO_PIN_NO_2
#define LCD1602_GPIO_D4        GPIO_PIN_NO_3
#define LCD1602_GPIO_D5        GPIO_PIN_NO_4
#define LCD1602_GPIO_D6        GPIO_PIN_NO_5
#define LCD1602_GPIO_D7        GPIO_PIN_NO_6
/** @} */

/* -------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

void LCD1602_Init(void);

void LCD1602_SendCommand(uint8_t cmd);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif  INC_LCD1602_H_
