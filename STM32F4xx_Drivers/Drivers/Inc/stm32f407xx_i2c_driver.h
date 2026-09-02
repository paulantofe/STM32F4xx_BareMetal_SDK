/**
 * @file 		stm32f407xx_i2c_driver.h
 * @brief 		Header file for I2C peripheral support
 *
 * @details
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

/* -------------------------- GENERIC MACROS -------------------------- */
/* -------------------------------------------------------------------- */


/* -------------------------- CONFIGURATION STRUCTURES -------------------------- */

/** @brief Configuration structure for I2C */
typedef struct {
	uint32_t I2C_SclSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint16_t I2C_FMDutyCycle;
} I2C_Config_t;

/** @brief Handle structure for I2C */
typedef struct {
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
} I2C_Handle_t;

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
