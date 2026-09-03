/**
 * @file 		stm32f407xx_i2c_driver.h
 * @brief 		Header file for I2C peripheral support
 *
 * @details     This header file provides the configuration and handle structures for the I2C peripheral on the MCU.
 *              It includes user macros for clock speeds (Standard and Fast Mode), ACK control, Fast Mode duty cycles
 *              and Repeated Start generation, along with API function prototypes for I2C initialization, master and
 *              slave data communication, peripheral control and interrupt handling.
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

/** @defgroup I2C_R_START      Enable/Disable Repeated Start
 * @{
 */
#define I2C_SR_EN              1
#define I2C_SR_DI              0
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

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

void I2C_PClkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

void I2C_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
