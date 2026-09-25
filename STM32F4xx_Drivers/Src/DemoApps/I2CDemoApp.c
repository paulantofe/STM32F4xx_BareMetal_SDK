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

char* time_to_string(RTC_Date_Time_t *time) {
	static char time_str[9];

	// Setting format
	time_str[2] = ':';
	time_str[5] = ':';
	time_str[8] = '\0';

	// Setting hours
	time_str[0] = (time->hours / 10) + '0';
	time_str[1] = (time->hours % 10) + '0';

	// Setting minutes
	time_str[3] = (time->minutes / 10) + '0';
	time_str[4] = (time->minutes % 10) + '0';

	// Setting seconds
	time_str[6] = (time->seconds / 10) + '0';
	time_str[7] = (time->seconds % 10) + '0';

	return time_str;
}

char* date_to_string(RTC_Date_Time_t *date) {
	static char date_str[11];

	// Setting format
	date_str[2] = '/';
	date_str[5] = '/';
	date_str[10] = '\0';

	// Setting date
	date_str[0] = (date->date / 10) + '0';
	date_str[1] = (date->date % 10) + '0';

	// Setting month
	date_str[3] = (date->month / 10) + '0';
	date_str[4] = (date->month % 10) + '0';

	// Setting year
	date_str[6] = '2';
	date_str[7] = '0';
	date_str[8] = (date->year / 10) + '0';
    date_str[9] = (date->year % 10) + '0';

    return date_str;
}

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

void LCD1602_Initialization(void) {
	LCD1602_Init();

	RTC_Date_Time_t read_date_time = { 0 };
	char *date_time;

	// Read and Display start time
	DS1307_GetTime(&read_date_time);
	LCD1602_SendString(time_to_string(&read_date_time));

	// Read and Display start date
	DS1307_GetDate(&read_date_time);
	LCD1602_SetCursor(1, 0);
	LCD1602_SendString(date_to_string(&read_date_time));
}

int main(void) {


	while (1) {

	}

	return 0;
}
