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

/** @defgroup I2C_SCL_SPEED    Speed of I2C Serial Clock
 * @{
 */
#define I2C_SCL_SPEED_SM       100000U   // Standard Mode 100 kHz
#define I2C_SCL_SPEED_FM       400000U   // Fast Mode     400 kHz
/** @} */

/** @defgroup I2C_ACK_CTRL     Enable/Disable ACK
 * @{
 */
#define I2C_ACK_EN             1
#define I2C_ACK_DI             0
/** @} */

/** @defgroup I2C_FM_DUTY      Duty Cycle for Fast Mode
 * @{
 */
#define I2C_FM_DUTY_2          0
#define I2C_FM_DUTY_16_9       1
/** @} */

/* -------------------------------------------------------------------- */


/* -------------------------- CONFIGURATION STRUCTURES -------------------------- */

/** @brief Configuration structure for I2C */
typedef struct {
	uint32_t I2C_SclSpeed;          /**< possible values from @ref I2C_SCL_SPEED    */
	uint8_t  I2C_DeviceAddress;     /**< configurable by the user                   */
	uint8_t  I2C_AckControl;        /**< possible values from @ref I2C_ACK_CTRL     */
	uint16_t I2C_FMDutyCycle;       /**< possible values from @ref I2C_FM_DUTY      */
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
