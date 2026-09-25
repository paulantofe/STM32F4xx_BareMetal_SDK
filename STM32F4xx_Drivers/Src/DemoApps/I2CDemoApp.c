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

void DS1307_Initialization(void) {
	DS1307_Init();

	RTC_Date_Time_t current_date_time = { 0 };

	// Set current time
	current_date_time.time_format = DS1307_TIME_FORMAT_24H;
	current_date_time.hours = 10;
	current_date_time.minutes = 30;
	current_date_time.seconds = 0;
	DS1307_SetTime(&current_date_time);

	// Set current date
	current_date_time.date = 25;
	current_date_time.day = FRIDAY;
	current_date_time.month = 9;
	current_date_time.year = 26;
	DS1307_SetDate(&current_date_time);

	// Set 1Hz Square Wave Output
    DS1307_ManageSquareWave(ENABLE);
    DS1307_SquareWaveFrequency(DS1307_SQ_1HZ);
}

int main(void) {


	while (1) {

	}

	return 0;
}
