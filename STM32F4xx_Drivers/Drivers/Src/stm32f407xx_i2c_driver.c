/**
 * @file 		stm32f407xx_i2c_driver.c
 * @brief 		Source file for I2C peripheral support
 *
 * @details
 */

#include "stm32f407xx_i2c_driver.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */

static uint16_t AHB_Prescaler[] = { 2, 4, 8, 16, 64, 128, 256, 512 };
static uint8_t  APB_Prescaler[] = { 2, 4, 8, 16 };

static uint32_t rcc_get_pclk1_value(void) {
	uint32_t pclk1, system_clk;
	uint8_t clk_source, temp, apb_pre;
	uint16_t ahb_pre;

	clk_source = (RCC->CFGR >> 2) & 0x3;

	if (clk_source == 0) {
		// HSI selected
		system_clk = 16000000;
	}
	else if (clk_source == 1) {
		// HSE selected
		system_clk = 8000000;
	}
	else if (clk_source == 2) {
		// PLL selected
		// Not supported by the custom driver layer
	}

	// AHB Clock Prescaler
	temp = (RCC->CFGR >> 4) & 0xF;
	if (temp < 8) {
		ahb_pre = 1;
	}
	else {
		ahb_pre = AHB_Prescaler[temp - 8];
	}

	// APB Clock Prescaler
	temp = (RCC->CFGR >> 10) & 0x7;
	if (temp < 4) {
		apb_pre = 1;
	}
	else {
		apb_pre = APB_Prescaler[temp - 4];
	}

	pclk1 = (system_clk / ahb_pre) / apb_pre;

	return pclk1;
}

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
 * @brief  Enable/Disable peripheral clock for a given I2C peripheral
 * @param  pI2Cx   Base address of I2C peripheral
 * @param  EnorDi  ENABLE/DISABLE macro
 * @retval None
 */
void I2C_PClkControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
	if (pI2Cx == NULL) { return; }

	if (EnorDi == ENABLE) {
		if (pI2Cx == I2C1) {
			I2C1_PCLK_EN();
		}
		else if (pI2Cx == I2C2) {
			I2C2_PCLK_EN();
		}
		else if (pI2Cx == I2C3) {
			I2C3_PCLK_EN();
		}
	} else {
		if (pI2Cx == I2C1) {
			I2C1_PCLK_DI();
		}
		else if (pI2Cx == I2C2) {
			I2C2_PCLK_DI();
		}
		else if (pI2Cx == I2C3) {
			I2C3_PCLK_DI();
		}
	}
}

/**
 * @brief  Initialize a I2C peripheral with the given settings
 * @param  pI2CHandle   Handle structure with desired settings
 * @retval None
 */
void I2C_Init(I2C_Handle_t *pI2CHandle) {

}

/**
 * @brief  De-initialize (reset) a I2C peripheral
 * @param  pI2Cx   Base address of I2C peripheral
 * @retval None
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx) {
	if (pI2Cx == NULL) { return; }

	if (pI2Cx == I2C1) {
		I2C1_REG_RESET();
	}
	else if (pI2Cx == I2C2) {
		I2C2_REG_RESET();
	}
	else if (pI2Cx == I2C3) {
		I2C3_REG_RESET();
	}
}

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
