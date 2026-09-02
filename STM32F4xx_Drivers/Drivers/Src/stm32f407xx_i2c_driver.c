/**
 * @file 		stm32f407xx_i2c_driver.c
 * @brief 		Source file for I2C peripheral support
 *
 * @details
 */

#include "stm32f407xx_spi_driver.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */
/* ----------------------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief
 * @param
 * @param
 * @retval
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

/**
 * @brief  Enable/Disable the given I2C peripheral
 * @param  pI2C    Base address of I2C peripheral
 * @param  EnorDi  ENABLE/DISABLE macro
 * @retval None
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
	if (pI2Cx == NULL) { return; }

	if (EnorDi == ENABLE) {
		pI2Cx->CR1 |= (1 << I2C_CR1_PE_POS);
	}
	else {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE_POS);
	}
}

/**
 * @brief
 * @param
 * @param
 * @retval
 */
void I2C_PClkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/**
 * @brief
 * @param
 * @retval
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);

/**
 * @brief
 * @param
 * @retval
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/**
 * @brief
 * @param
 * @param
 * @param
 * @param
 * @param
 * @retval
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief
 * @param
 * @param
 * @param
 * @param
 * @param
 * @retval
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief
 * @param
 * @param
 * @retval
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);

/**
 * @brief
 * @param
 * @retval
 */
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

/**
 * @brief
 * @param
 * @param
 * @retval
 */
void I2C_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);

/**
 * @brief
 * @param
 * @param
 * @retval
 */
void I2C_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);

/* ----------------------------------------------------------------------------------- */
