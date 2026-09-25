/**
 * @file 		I2CDemoApp.c
 * @brief 	    STM32F407xx Bare-Metal I2C driver demo
 *
 * @details		This demo uses a DS1307 RTC module which communicates in slave mode with MCU via I2C Communication.
 *              The MCU fetches the current date and time every 1 second (1Hz provided by RTCs SQ Pin) and displays
 *              it on LCD Display.
 *
 * @note        TO RUN THIS DEMO: Ensure that the default main.c is excluded from build and this file is included in the project`s
 *              source path.
 *
 * Hardware Setup:
 * - DS1307 RTC Module with I2C Communication
 * - LCD1602 Display
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"

#include "ds1307.h"
#include "lcd1602.h"

int main(void) {


	while (1) {

	}

	return 0;
}
