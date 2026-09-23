/**
 * @file 		ds1307.h
 * @brief 		Board Support Package (BSP) driver header for the DS1307 Real-Time Clock (RTC).
 *
 * @details     This header file provides register mappings, the I2C device address and configuration
 *              macros specific to the DS1307 RTC chip. It also exposes the public API prototypes for
 *              initializing the module, managing the square-wave (SQ) output, and reading or writing
 *              time and date data over the I2C bus.
 */

#ifndef INC_DS1307_H_
#define INC_DS1307_H_

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"

/* -------------------------- GENERIC MACROS -------------------------- */

/**
 * @note PB6 and PB9 have on board 4.7kΩ pull-up resistors.
 *       Maximum I2C speed supported by DS1307 is 100 kHz (standard mode)
 * @defgroup DS1307_APP_CFG    Application Configurable Items
 * @{
 */
#define DS1307_I2C             I2C1
#define DS1307_I2C_GPIO_PORT   GPIOB
#define DS1307_I2C_SDA_PIN     GPIO_PIN_NO_9
#define DS1307_I2C_SCL_PIN     GPIO_PIN_NO_6
#define DS1307_I2C_SPEED       I2C_SCL_SPEED_SM
#define DS1307_I2C_PUPD        GPIO_NO_PUPD
/** @} */

/**
 * @defgroup DS1307_I2C_ADDR   I2C address of DS1307
 * @{
 */
#define DS1307_I2C_ADDR        0x68
/** @} */

/**
 * @defgroup DS1307_REG        Registers Addresses of DS1307 chip
 * @{
 */
#define DS1307_ADDR_SEC        0x00
#define DS1307_ADDR_MIN        0x01
#define DS1307_ADDR_HRS        0x02
#define DS1307_ADDR_DAY        0x03
#define DS1307_ADDR_DATE       0x04
#define DS1307_ADDR_MONTH      0x05
#define DS1307_ADDR_YEAR       0x06
#define DS1307_ADDR_CTRL       0x07
/** @} */

/**
 * @defgroup DS1307_CONFIG     Configuration Macros
 * @{
 */
#define DS1307_TIME_FORMAT_12H_AM   0
#define DS1307_TIME_FORMAT_12H_PM   1
#define DS1307_TIME_FORMAT_24H      2
/** @} */

/**
 * @defgroup DS1307_DAYS       Days of the week
 * @{
 */
typedef enum {
	SUNDAY    = 1,
	MONDAY    = 2,
	TUESDAY   = 3,
	WEDNESDAY = 4,
	THURSDAY  = 5,
	FRIDAY    = 6,
	SATURDAY  = 7
} DS1307_Days_t;
/** @} */

/**
 * @defgroup DS1307_BIT_POS    Bit position definitions for DS1307 registers
 * @{
 */
#define DS1307_SEC_CH_POS      7
#define DS1307_HRS_FORMAT_POS  6
#define DS1307_HRS_AM_PM_POS   5
#define DS1307_CTRL_SQWE_POS   4
#define DS1307_CTRL_RS0_POS    0
#define DS1307_CTRL_RS1_POS    1
/** @} */

/**
 * @defgroup DS1307_SQ_FREQ    Frequency of the square wave output
 * @{
 */
#define DS1307_SQ_1HZ          0
#define DS1307_SQ_4_096KHZ     1
#define DS1307_SQ_8_192KHZ     2
#define DS1307_SQ_32_768KHZ    3
/** @} */

/* -------------------------------------------------------------------- */


/* -------------------------- DATA STRUCTURES -------------------------- */

/** @brief Structure to hold the decoded date and time */
typedef struct {
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t day;               /**< possible values from @ref DS1307_DAYS         */
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t time_format;       /**< possible values from @ref DS1307_CONFIG       */
} RTC_Date_Time_t;

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

void DS1307_Init(void);
void DS1307_SetTime(RTC_Date_Time_t *pRTC_Time);
void DS1307_SetDate(RTC_Date_Time_t *pRTC_Date);

void DS1307_GetTime(RTC_Date_Time_t *pRTC_Time);
void DS1307_GetDate(RTC_Date_Time_t *pRTC_Date);

void DS1307_ManageSquareWave(uint8_t EnorDi);
void DS1307_SquareWaveFrequency(uint8_t Freq);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
