/**
 * @file 		stm32f407xx_i2c_driver.c
 * @brief 		Source file for I2C peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the I2C peripherals on the MCU. It includes the functional
 *              code for peripheral clock control, I2C initialization and de-initialization, data transmission, as well as interrupt
 *              configuration and handling.
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

static void i2c_execute_address_phase(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddress, uint8_t RnW) {
	SlaveAddress = SlaveAddress << 1;

	if (RnW == 1) {
		SlaveAddress |= 1;
	}
	else {
		SlaveAddress &= ~1;
	}

	pI2Cx->DR = SlaveAddress;
}

static void i2c_clear_addr_flag(I2C_RegDef_t *pI2Cx) {
	(void) pI2Cx->SR1;
	(void) pI2Cx->SR2;
}

static void i2c_wait_on_flag_timeout(I2C_RegDef_t *pI2Cx, uint32_t FlagName, uint8_t Status) {
	uint32_t timeout = 500000;

	while (I2C_GetFlagStatus(pI2Cx, FlagName) == Status) {
		timeout--;
		if (timeout == 0) { return; }
	}
}

static void i2c_close_tx(I2C_Handle_t *pI2CHandle) {
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN_POS);
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN_POS);
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN_POS);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}

static void i2c_close_rx(I2C_Handle_t *pI2CHandle) {
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN_POS);
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN_POS);
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN_POS);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;

	if (pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_EN) {
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_ACK_POS);
	}
}

static void i2c_handle_rxne_ev(I2C_Handle_t *pI2CHandle) {
	if (pI2CHandle->RxSize == 1) {
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen--;
	}
	else if (pI2CHandle->RxSize > 1) {
		if (pI2CHandle->RxLen == 2) {
		// Disable ACK
		pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_POS);
		}
		else if (pI2CHandle->RxLen == 1) {
			if (pI2CHandle->Sr == I2C_SR_DI) {
				pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);
			}
		}
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen--;
		pI2CHandle->pRxBuffer++;
	}
	if (pI2CHandle->RxLen == 0) {
		i2c_close_rx(pI2CHandle);

		I2C_ApplicationEventCallback(pI2CHandle, I2C_EVENT_RX_CMPLT);
	}
}

/* ----------------------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Check status for a specific I2C flag
 * @param  pI2Cx    Base address of I2C peripheral
 * @param  FlagName Macro of the flag to check @ref I2C_FLAGS
 * @retval FLAG_SET or FLAG_RESET macro
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName) {
	if (pI2Cx == NULL) { return FLAG_RESET; }

    return (pI2Cx->SR1 & FlagName) ? FLAG_SET : FLAG_RESET;
}

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
	uint32_t temp_reg = 0;

	// ACK Control Bit
	temp_reg |= (pI2CHandle->I2C_Config.I2C_AckControl << I2C_CR1_ACK_POS);
	pI2CHandle->pI2Cx->CR1 = temp_reg;

	// FREQ Field of CR1
	temp_reg = 0;
	temp_reg |= rcc_get_pclk1_value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (temp_reg & 0x3F);

	// Device Own Address
	temp_reg = 0;
	temp_reg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1);
	temp_reg |= (1 << 14); // Reference Manual mentions that this bit must be programmed to 1, no explanation
	pI2CHandle->pI2Cx->OAR1 = temp_reg;

	// CCR Calculations
	uint16_t ccr_value = 0;
	temp_reg = 0;
	if (pI2CHandle->I2C_Config.I2C_SclSpeed == I2C_SCL_SPEED_SM) {
		// Standard Mode
		ccr_value = rcc_get_pclk1_value() / (2 * pI2CHandle->I2C_Config.I2C_SclSpeed);
		temp_reg = (ccr_value & 0xFFF);
	}
	else {
		// Fast Mode
		temp_reg = (1 << I2C_CCR_F_S_POS);
		temp_reg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY_POS);

		if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2) {
			ccr_value = rcc_get_pclk1_value() / (3 * pI2CHandle->I2C_Config.I2C_SclSpeed);
		}
		else {
			ccr_value = rcc_get_pclk1_value() / (25 * pI2CHandle->I2C_Config.I2C_SclSpeed);
		}

		temp_reg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = temp_reg;

	// TRISE Configuration
	if (pI2CHandle->I2C_Config.I2C_SclSpeed == I2C_SCL_SPEED_SM) {
		// Standard Mode: maximum rise time is 1000 ns
		temp_reg = (rcc_get_pclk1_value() / 1000000U) + 1;
	}
	else {
		// Fast Mode: maximum rise time is 300 ns
		temp_reg = ((rcc_get_pclk1_value() / 1000000U) * 300) / 1000 +  1;
	}
	pI2CHandle->pI2Cx->TRISE = (temp_reg & 0x3F);
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
 * @brief  Transmit data using I2C protocol
 * @param  pI2CHandle    Handle structure
 * @param  pTxBuffer     Pointer to transmission buffer
 * @param  Len           Length of the transmission in bytes
 * @param  SlaveAddr     Address of the slave to transmit to
 * @param  Sr            I2C_SR_EN/I2C_SR_DI macro
 * @retval None
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
	// Generate Start Condition
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_START_POS);

	// Wait until Start Condition is generated
	i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_SB_FLAG, FLAG_RESET);

	// Send Slave Address along with R/nW bit set to 0 (8 bits in total)
    i2c_execute_address_phase(pI2CHandle->pI2Cx, SlaveAddr, 0);

    // Wait until Address Phase is over
    i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_ADDR_FLAG, FLAG_RESET);

    // Clear ADDR Flag. Note: SCL is stretched until ADDR Flag is cleared
    i2c_clear_addr_flag(pI2CHandle->pI2Cx);

    // Send data until Len is 0
    while (Len > 0) {
    	// Wait until Tx buffer is empty
    	i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_TXE_FLAG, FLAG_RESET);

    	pI2CHandle->pI2Cx->DR = *pTxBuffer;
    	pTxBuffer++;
    	Len--;
    }

    // Wait until Tx buffer is empty and BTF Flag is set (Byte Transfer Finished)
    i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_TXE_FLAG, FLAG_RESET);
    i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_BTF_FLAG, FLAG_RESET);

    if (Sr == I2C_SR_DI) {
    	// Repeated Start is deactivated. Generate stop condition
    	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);
    }
}

/**
 * @brief  Receive data using I2C protocol
 * @param  pI2CHandle     Handle structure
 * @param  pRxBuffer      Pointer to reception buffer
 * @param  Len            Length of the reception in bytes
 * @param  SlaveAddr      Address of the slave to receive from
 * @param  Sr             I2C_SR_EN/I2C_SR_DI macro
 * @retval None
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
	// Generate Start Condition
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_START_POS);

	// Wait until Start Condition is generated
	i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_SB_FLAG, FLAG_RESET);

	// Send Slave Address along with R/nW bit set to 1 (8 bits in total)
	i2c_execute_address_phase(pI2CHandle->pI2Cx, SlaveAddr, 1);

	// Wait until Address Phase is over
	i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_ADDR_FLAG, FLAG_RESET);

	// Procedure to read only one byte of data from slave
	if (Len == 1) {
		// Disable Acking
		pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_POS);

		// Clear ADDR Flag. Note: SCL is stretched until ADDR Flag is cleared
		i2c_clear_addr_flag(pI2CHandle->pI2Cx);

		if (Sr == I2C_SR_DI) {
			// Repeated Start is deactivated. Generate stop condition
		    pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);
		}

		// Wait until Rx buffer is not empty
		i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_RXNE_FLAG, FLAG_RESET);

		// Read data into pRxBuffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

	else if (Len > 1) {
		// Clear ADDR Flag. Note: SCL is stretched until ADDR Flag is cleared
	    i2c_clear_addr_flag(pI2CHandle->pI2Cx);

	    while (Len > 0) {
	    	i2c_wait_on_flag_timeout(pI2CHandle->pI2Cx, I2C_RXNE_FLAG, FLAG_RESET);

	    	if (Len == 2) {
	    		// Disable Acking
	    		pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_POS);

	    		if (Sr == I2C_SR_DI) {
	    		    // Repeated Start is deactivated. Generate stop condition
	    		   	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);
	    	    }
	    	}

	    	// Read data into pRxBuffer
	    	*pRxBuffer = pI2CHandle->pI2Cx->DR;
	    	pRxBuffer++;
	    	Len--;
	    }
	}

	// Restore ACK Control to initial configuration
	if (pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_EN) {
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_ACK_POS);
    }
}

/**
 * @brief  Transmit data using I2C protocol (non-blocking mode)
 * @param  pI2CHandle    Handle structure
 * @param  pTxBuffer     Pointer to transmission buffer
 * @param  Len           Length of the transmission in bytes
 * @param  SlaveAddr     Address of the slave to transmit to
 * @param  Sr            I2C_SR_EN/I2C_SR_DI macro
 * @retval I2C Peripheral state before API call:
 *         - I2C_READY: transmission started
 *         - I2C_BUSY_IN_TX/I2C_BUSY_IN_RX: peripheral was busy. Data is not transmitted
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
	uint8_t state = pI2CHandle->TxRxState;

	if((state != I2C_BUSY_IN_TX) && (state != I2C_BUSY_IN_RX)) {
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		// Enable ITBUFEN, ITEVTEN and ITERREN
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN_POS) |
				                  (1 << I2C_CR2_ITEVTEN_POS) |
								  (1 << I2C_CR2_ITERREN_POS);

		// Generate Start Condition
	    pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_START_POS);
	}

	return state;
}

/**
 * @brief  Receive data using I2C protocol (non-blocking mode)
 * @param  pI2CHandle    Handle structure
 * @param  pRxBuffer     Pointer to reception buffer
 * @param  Len           Length of the reception in bytes
 * @param  SlaveAddr     Address of the slave to receive from
 * @param  Sr            I2C_SR_EN/I2C_SR_DI macro
 * @retval I2C Peripheral state before API call:
 *         - I2C_READY: transmission started
 *         - I2C_BUSY_IN_TX/I2C_BUSY_IN_RX: peripheral was busy. Data is not transmitted
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
	uint8_t state = pI2CHandle->TxRxState;

	if( (state != I2C_BUSY_IN_TX) && (state != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		// Enable ITBUFEN, ITEVTEN and ITERREN
        pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN_POS) |
        		                  (1 << I2C_CR2_ITEVTEN_POS) |
								  (1 << I2C_CR2_ITERREN_POS);

		// Generate Start Condition
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_START_POS);
	}

	return state;
}

/**
 * @brief  Configure an interrupt for I2C peripheral
 * @param  Number of the interrupt request from IRQn_Type enum
 * @param  ENABLE/DISABLE macro
 * @retval None
 */
void I2C_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi) {
	if (IRQNumber > 81) { return; }

	if (EnorDi == ENABLE) {
		NVIC_ISER_BASEADDR[IRQNumber / 32] = (1 << (IRQNumber % 32));
	}
	else {
		NVIC_ICER_BASEADDR[IRQNumber / 32] = (1 << (IRQNumber % 32));
	}
}

/**
 * @brief  Set interrupt priority for I2C interrupt
 * @param  Number of the interrupt request from IRQn_Type enum
 * @param  Priority of the interrupt
 * @retval None
 */
void I2C_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority) {
	if (IRQNumber > 81 || IRQPriority > 15) { return; }

	uint8_t iprx = IRQNumber / 4;
	uint8_t iprxSection = IRQNumber % 4;
	uint8_t shiftAmount = 8 * iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);

	NVIC_IPR_BASEADDR[iprx] &= ~(0xFF << (8 * iprxSection));
	NVIC_IPR_BASEADDR[iprx] |= (IRQPriority << shiftAmount);
}

/**
 * @brief  Manage interrupt events of I2C peripheral
 * @param  Handle structure
 * @retval None
 */
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle) {
	uint32_t temp1, temp2, temp3;
	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN_POS);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN_POS);

	uint8_t app_state = pI2CHandle->TxRxState;

	// Handle interrupt generated by SB event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB_POS);
	if (temp1 && temp3) {
		// SB Flag is set. Execute Address Phase
		if (app_state == I2C_BUSY_IN_TX) {
			i2c_execute_address_phase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, 0);
		}
		else if (app_state == I2C_BUSY_IN_RX){
			i2c_execute_address_phase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, 1);
		}
	}

	// Handle interrupt generated by ADDR event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR_POS);
	if (temp1 && temp3) {
		// ADDR Flag is set. Clear ADDR Flag
		if (app_state == I2C_BUSY_IN_RX) {
			if (pI2CHandle->RxSize == 1) {
				// Disable ACK before clearing ADDR FLag
				pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_POS);

				if (pI2CHandle->Sr == I2C_SR_DI) {
					pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);
				}
			}
		}

		i2c_clear_addr_flag(pI2CHandle->pI2Cx);
	}

	// Handle interrupt generated by BTF event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF_POS);
	if (temp1 && temp3) {
		// BTF Flag is set
		if (app_state == I2C_BUSY_IN_TX) {
			if (pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE_POS)) {
				if (pI2CHandle->TxLen == 0) {
					// Generate Stop Condition
					if (pI2CHandle->Sr == I2C_SR_DI) {
						pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);
					}

					// Reset handle structure members
					i2c_close_tx(pI2CHandle);

					I2C_ApplicationEventCallback(pI2CHandle, I2C_EVENT_TX_CMPLT);
				}
			}
		}
		else if (app_state == I2C_BUSY_IN_RX) {
			if (pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE_POS)) {
				// Nothing to do
			}
		}
	}

	// Handle interrupt generated by STOPF event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF_POS);
	if (temp1 && temp3) {
		// STOPF Flag is set
		// N/A in master mode
	}

	// Handle interrupt generated by TXE Event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE_POS);
	if (temp1 && temp2 && temp3) {
		// TXE Flag is set. Perform Data Transmission
		if (app_state == I2C_BUSY_IN_TX) {
			if (pI2CHandle->TxLen > 0) {
				pI2CHandle->pI2Cx->DR = *pI2CHandle->pTxBuffer;
				pI2CHandle->TxLen--;
				pI2CHandle->pTxBuffer++;
			}
		}
	}

	// Handle interrupt generated by RXNE Event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE_POS);
	if (temp1 && temp2 && temp3) {
		// RXNE Flag is set. Perform Data Reception
		if (app_state == I2C_BUSY_IN_RX) {
			i2c_handle_rxne_ev(pI2CHandle);
		}
	}
}

/**
 * @brief  Manage interrupt errors of I2C peripheral
 * @param  Handle structure
 * @retval None
 */
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle) {
	uint32_t temp1, temp2;

    temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITERREN_POS);

	// Handle Bus Error
	temp2 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BERR_POS);
	if(temp1 && temp2 ) {
		// Clear Buss Error Flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_BERR_POS);

		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	}

	// Handle Arbitration Lost Error Flag
	temp2 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ARLO_POS);
	if(temp1 && temp2) {
		// Clear Arbitration Lost Error Flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO_POS);

		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}

    // Handle Acknowledge Failure Error Flag
	temp2 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_AF_POS);
	if(temp1 && temp2) {
		// Clear Acknowledge Failure Error Flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_AF_POS);

		// Generate Stop Condition
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOP_POS);

		i2c_close_rx(pI2CHandle);
		i2c_close_tx(pI2CHandle);

		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

	// Handle Overrun/Underrun Error Flag
	temp2 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_OVR_POS);
	if(temp1 && temp2) {
		// Clear Overrun/Underrun Error Flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_OVR_POS);

		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

	// Handle Timeout Error Flag
	temp2 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TIMEOUT_POS);
	if(temp1 && temp2) {
		// Clear Timeout Error Flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_TIMEOUT_POS);

		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}
}

/* ----------------------------------------------------------------------------------- */
