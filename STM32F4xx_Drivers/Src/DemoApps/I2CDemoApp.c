/**
 * @file 		I2CDemoApp.c
 * @brief 	    STM32F407xx Bare-Metal I2C driver demo
 *
 * @details		This demo uses a DS1307 RTC module which communicates in slave mode with MCU via I2C Communication.
 *              The MCU fetches the current date and time every 1 second (1Hz provided by RTCs SQ Pin) and displays
 *              it on LCD Display.
 *
 * @note        TO RUN THIS DEMO: Ensure that the default main.c is excluded from build and this file is included in the project's
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

__vo uint8_t update_display = false;

char* Time_to_String(RTC_Date_Time_t *time) {
	static char time_str[11];

	// Setting format
	time_str[2] = ':';
	time_str[5] = ':';

	// Setting hours
	time_str[0] = (time->hours / 10) + '0';
	time_str[1] = (time->hours % 10) + '0';

	// Setting minutes
	time_str[3] = (time->minutes / 10) + '0';
	time_str[4] = (time->minutes % 10) + '0';

	// Setting seconds
	time_str[6] = (time->seconds / 10) + '0';
	time_str[7] = (time->seconds % 10) + '0';

	// Add AM/PM or terminate for 24H format
	if (time->time_format == DS1307_TIME_FORMAT_24H) {
		time_str[8] = '\0';
	}
	else if (time->time_format == DS1307_TIME_FORMAT_12H_AM) {
		time_str[8] = 'A';
		time_str[9] = 'M';
		time_str[10] = '\0';
	}
	else if (time->time_format == DS1307_TIME_FORMAT_12H_PM) {
		time_str[8] = 'P';
		time_str[9] = 'M';
		time_str[10] = '\0';
	}

	return time_str;
}

char* Date_to_String(RTC_Date_Time_t *date) {
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

void Rtc_Init(void) {
	DS1307_Init();

	RTC_Date_Time_t current_date_time = { 0 };

	// Set current time
	current_date_time.time_format = DS1307_TIME_FORMAT_12H_AM;
	current_date_time.hours = 11;
	current_date_time.minutes = 25;
	current_date_time.seconds = 40;
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

void Display_Init(void) {
	LCD1602_Init();

	RTC_Date_Time_t read_date_time = { 0 };

	// Read and Display start time
	DS1307_GetTime(&read_date_time);
	LCD1602_SetCursor(1, 1);
	LCD1602_SendString(Time_to_String(&read_date_time));

	// Read and Display start date
	DS1307_GetDate(&read_date_time);
	LCD1602_SetCursor(2, 1);
	LCD1602_SendString(Date_to_String(&read_date_time));
}

void SQInterrupt_Config(void) {
    // Set an interrupt on C12
	// Note: on my RTC module the SQ pin has a pull-up resistor soldered, so no internal pull-up required

	GPIO_PClkControl(GPIOC, ENABLE);

	GPIO_Handle_t sq_it = { 0 };
	sq_it.pGPIOx = GPIOC;
	sq_it.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	sq_it.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;

	GPIO_Init(&sq_it);
	GPIO_IRQInterruptConfig(EXTI15_10_IRQn, ENABLE);
	GPIO_IRQPriorityConfig(EXTI15_10_IRQn, NVIC_IRQ_PR0);
}

void Update_Display(void) {
	RTC_Date_Time_t read_date_time = { 0 };

	// Read and Display start time
	DS1307_GetTime(&read_date_time);
	LCD1602_SetCursor(1, 1);
	LCD1602_SendString(Time_to_String(&read_date_time));

	// Read and Display start date
	DS1307_GetDate(&read_date_time);
	LCD1602_SetCursor(2, 1);
	LCD1602_SendString(Date_to_String(&read_date_time));
}

int main(void) {
    SQInterrupt_Config();
	Rtc_Init();
	Display_Init();

	while (true) {
		if (update_display == true) {
			update_display = false;

			Update_Display();
		}
	}

	return 0;
}

void EXTI15_10_IRQHandler(void) {
	GPIO_IRQHandling(GPIO_PIN_NO_12);

	update_display = true;
}
