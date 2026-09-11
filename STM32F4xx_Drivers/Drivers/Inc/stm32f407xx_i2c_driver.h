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

/**
 * @defgroup I2C_SCL_SPEED    Speed of I2C Serial Clock
 * @{
 */
#define I2C_SCL_SPEED_SM       100000U   // Standard Mode 100 kHz
#define I2C_SCL_SPEED_FM       400000U   // Fast Mode     400 kHz
/** @} */

/**
 * @defgroup I2C_ACK_CTRL     Enable/Disable ACK
 * @{
 */
#define I2C_ACK_EN             1
#define I2C_ACK_DI             0
/** @} */

/**
 * @defgroup I2C_FM_DUTY      Duty Cycle for Fast Mode
 * @{
 */
#define I2C_FM_DUTY_2          0
#define I2C_FM_DUTY_16_9       1
/** @} */

/**
 * @defgroup I2C_FLAGS         Status Flags
 * @{
 */
#define I2C_TXE_FLAG           (1 << I2C_SR1_TXE_POS)
#define I2C_RXNE_FLAG          (1 << I2C_SR1_RXNE_POS)
#define I2C_SB_FLAG            (1 << I2C_SR1_SB_POS)
#define I2C_ADDR_FLAG          (1 << I2C_SR1_ADDR_POS)
#define I2C_BTF_FLAG           (1 << I2C_SR1_BTF_POS)
#define I2C_STOPF_FLAG         (1 << I2C_SR1_STOPF_POS)
#define I2C_BERR_FLAG          (1 << I2C_SR1_BERR_POS)
#define I2C_ARLO_FLAG          (1 << I2C_SR1_ARLO_POS)
#define I2C_AF_FLAG            (1 << I2C_SR1_AF_POS)
#define I2C_OVR_FLAG           (1 << I2C_SR1_OVR_POS)
#define I2C_TIMEOUT_FLAG       (1 << I2C_SR1_TIMEOUT_POS)
#define I2C_BUSY_FLAG          (1 << I2C_SR2_BUSY_POS)
/** @} */

/** @defgroup I2C_SR_CTRL      Enable/Disable Repeated Start
 * @{
 */
#define I2C_SR_EN              1
#define I2C_SR_DI              0
/** @} */

/** @defgroup I2C_STATES       I2C Application States
 * @{
 */
#define I2C_READY              0
#define I2C_BUSY_IN_RX         1
#define I2C_BUSY_IN_TX         2
/** @} */

/*
 * @defgroup I2C_EVENTS        I2C Application Events
 * @{
 */
#define I2C_EVENT_TX_CMPLT     0
#define I2C_EVENT_RX_CMPLT     1
#define I2C_EVENT_STOP         2
#define I2C_EVENT_DATA_REQ     3
#define I2C_EVENT_DATA_RCV     4
/** @} */

/*
 * @defgroup I2C_ERRORS        I2C Application Errors
 * @{
 */
#define I2C_ERROR_BERR         0   // Bus Error
#define I2C_ERROR_ARLO         1   // Arbitration Lost Error
#define I2C_ERROR_AF           2   // Acknowledge Failure Error
#define I2C_ERROR_OVR          3   // Overrun/Underrun Error
#define I2C_ERROR_TIMEOUT      4   // Timeout Error
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

	uint8_t      *pTxBuffer;
	uint8_t      *pRxBuffer;
	uint32_t     TxLen;
	uint32_t     RxLen;
	uint8_t      TxRxState;
	uint8_t      DevAddr;
	uint32_t     RxSize;
	uint8_t      Sr;
} I2C_Handle_t;

/* ------------------------------------------------------------------------------ */


/* -------------------------- APPLICATION CALL BACKS -------------------------- */

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

void I2C_PClkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

void I2C_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
