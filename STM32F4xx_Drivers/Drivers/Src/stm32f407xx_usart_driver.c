/**
 * @file 		stm32f407xx_usart_driver.c
 * @brief 		Source file for USART peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the USART/UART peripherals on the MCU. It includes the functional
 *              code for peripheral clock control, USART initialization and de-initialization, data transmission, as well as interrupt
 *              configuration and handling.
 */

#include "stm32f407xx_usart_driver.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */
/* ----------------------------------------------------------------------------------- */


/* -------------------------- APPLICATION CALL BACKS WEAK IMPLEMENTATION -------------------------- */

/**
 * @brief  Application event callback function
 * @param  pUSARTHandle   Pointer to the USART handle structure
 * @param  AppEv          Application event type macro
 * @retval None
 */
__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv) {
	// This is a weak implementation. The application may override this function
}

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Check status for a specific USART flag
 * @param  pUSARTx    Base address of USART peripheral
 * @param  FlagName   Macro of the flag to check @ref USART_FLAGS
 * @retval FLAG_SET or FLAG_RESET macro
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName) {
	if (pUSARTx == NULL) { return FLAG_RESET; }

	return (pUSARTx->SR & FlagName) ? FLAG_SET : FLAG_RESET;
}

/**
 * @brief  Enable/Disable the given USART peripheral
 * @param  pUSARTx    Base address of USART peripheral
 * @param  EnorDi     ENABLE/DISABLE macro
 * @retval None
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi) {
	if (pUSARTx == NULL) { return; }

	if (EnorDi == ENABLE) {
		pUSARTx->CR1 |= (1 << USART_CR1_UE_POS);
	}
	else {
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE_POS);
	}
}

/**
 * @brief  Enable/Disable peripheral clock for a given USART peripheral
 * @param  pUSARTx   Base address of USART peripheral
 * @param  EnorDi    ENABLE/DISABLE macro
 * @retval None
 */
void USART_PClkControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi) {
	if (pUSARTx == NULL) { return; }

	if (EnorDi == ENABLE) {
		if (pUSARTx == UART4) {
			UART4_PCLK_EN();
		}
		else if (pUSARTx == UART5) {
			UART5_PCLK_EN();
		}
		else if (pUSARTx == USART1) {
			USART1_PCLK_EN();
		}
		else if (pUSARTx == USART2) {
			USART2_PCLK_EN();
		}
		else if (pUSARTx == USART3) {
			USART3_PCLK_EN();
		}
		else if (pUSARTx == USART6) {
			USART6_PCLK_EN();
		}
	}
	else {
		if (pUSARTx == UART4) {
			UART4_PCLK_DI();
		}
		else if (pUSARTx == UART5) {
			UART5_PCLK_DI();
		}
		else if (pUSARTx == USART1) {
			USART1_PCLK_DI();
		}
		else if (pUSARTx == USART2) {
			USART2_PCLK_DI();
		}
		else if (pUSARTx == USART3) {
			USART3_PCLK_DI();
		}
		else if (pUSARTx == USART6) {
			USART6_PCLK_DI();
		}
	}
}

/**
 * @brief  Initialize a USART peripheral with the given settings
 * @param  pUSARTHandle   Handle structure with desired settings
 * @retval None
 */
void USART_Init(USART_Handle_t *pUSARTHandle) {
	if (pUSARTHandle->pUSARTx == NULL) { return; }

	uint32_t temp_reg = 0x0000;

	// Mode Configuration
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TX_ONLY) {
		temp_reg |= (1 << USART_CR1_TE_POS);
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_RX_ONLY) {
		temp_reg |= (1 << USART_CR1_RE_POS);
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_FD) {
		temp_reg |= (1 << USART_CR1_TE_POS);
		temp_reg |= (1 << USART_CR1_RE_POS);
	}

	// Parity Control Configuration
	if (pUSARTHandle->USART_Config.USART_ParityControl != USART_PAR_DI) {
		temp_reg |= (1 << USART_CR1_PCE_POS);
		if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PAR_EN_EVEN) {
			// PS bit is reset
			// This is already true since temp_reg is initialized with 0
		}
		else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PAR_EN_ODD) {
			temp_reg |= (1 << USART_CR1_PS_POS);
		}
	}

	// Word Length Configuration
	if (pUSARTHandle->USART_Config.USART_WordLen == USART_WORD_9BITS) {
		// If word length is 8 bits then M bit is reset
		temp_reg |= (1 << USART_CR1_M_POS);
	}

	// Configure USART_CR1 Register
	pUSARTHandle->pUSARTx->CR1 = temp_reg;

	// Stop Bits Configuration
	temp_reg = pUSARTHandle->pUSARTx->CR2;
	temp_reg &= ~(0x3 << USART_CR2_STOP_POS);
	temp_reg |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP_POS);
	pUSARTHandle->pUSARTx->CR2 = temp_reg;

	// Hardware Flow Control Configuration
	temp_reg = pUSARTHandle->pUSARTx->CR3;
	temp_reg &= ~(1 << USART_CR3_CTSE_POS);
	temp_reg &= ~(1 << USART_CR3_RTSE_POS);
	if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTS ||
	    pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTS_RTS) {
		temp_reg |= (1 << USART_CR3_CTSE_POS);
	}
	if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_RTS ||
	         pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTS_RTS) {
		temp_reg |= (1 << USART_CR3_RTSE_POS);
	}
	pUSARTHandle->pUSARTx->CR3 = temp_reg;

	// Baud Rate Configuration
	// to be continued
}

/**
 * @brief  Transmit data using USART/UART protocol
 * @param  pUSARTHandle    Handle structure
 * @param  pTxBuffer       Pointer to transmission buffer
 * @param  Len             Number of data frames (words)
 * @retval None
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len) {
	uint16_t *pData;

	for (uint32_t i = 0; i < Len; i++) {
		// Wait until TXE is set
		while (USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE) == FLAG_RESET);

		if (pUSARTHandle->USART_Config.USART_WordLen == USART_WORD_9BITS) {
			// Load USART_DR with 2 bytes masking the bits other than the first 9 bits
			pData = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pData & (uint16_t) 0x01FF);

			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PAR_DI) {
				pTxBuffer += 2;
			}
			else {
				pTxBuffer++;
			}
		}
		else {
			// 8-bit data format
			pUSARTHandle->pUSARTx->DR = *pTxBuffer;
			pTxBuffer++;
		}
	}

	// Wait until transission is completed
	while (USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC) == FLAG_RESET);
}

/**
 * @brief  Configure an interrupt for USART peripheral
 * @param  IRQNumber   Number of the interrupt request from IRQn_Type enum
 * @param  EnorDi      ENABLE/DISABLE macro
 * @retval None
 */
void USART_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi) {
	if (IRQNumber > 81) { return; }

	if (EnorDi == ENABLE) {
		NVIC_ISER_BASEADDR[IRQNumber / 32] = (1 << (IRQNumber % 32));
	}
	else {
		NVIC_ICER_BASEADDR[IRQNumber / 32] = (1 << (IRQNumber % 32));
	}
}

/**
 * @brief  Set interrupt priority for USART interrupt
 * @param  IRQNumber     Number of the interrupt request from IRQn_Type enum
 * @param  IRQPriority   Priority of the interrupt
 * @retval None
 */
void USART_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority) {
	if (IRQNumber > 81 || IRQPriority > 15) { return; }

	uint8_t iprx = IRQNumber / 4;
	uint8_t iprxSection = IRQNumber % 4;
	uint8_t shiftAmount = 8 * iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);

	NVIC_IPR_BASEADDR[iprx] &= ~(0xFF << (8 * iprxSection));
	NVIC_IPR_BASEADDR[iprx] |= (IRQPriority << shiftAmount);
}

/* ----------------------------------------------------------------------------------- */
