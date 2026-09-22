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

#include "stm32f407xx_i2c_driver.h"

/* -------------------------- GENERIC MACROS -------------------------- */

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
#define DS1307_I2C_ADDR        0x68
#define DS1307_TIME_FORMAT_12H 1
#define DS1307_TIME_FORMAT_24H 0
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

/* -------------------------------------------------------------------- */

#endif
