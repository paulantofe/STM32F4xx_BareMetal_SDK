/**
 * @file 		stm32f407xx_usart_driver.c
 * @brief 		Source file for USART peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the USART/UART peripherals on the MCU. It includes the functional
 *              code for peripheral clock control, USART initialization and de-initialization, data transmission, as well as interrupt
 *              configuration and handling.
 */

#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */

static void usart_set_baud_rate(USART_RegDef_t *pUSARTx, uint32_t BaudRate) {
	uint32_t temp_reg = 0;

	// Get value of APBx bus clock
	uint32_t PCLKx;
	if (pUSARTx == USART1 || pUSARTx == USART6) {
		PCLKx = RCC_GetPCLK2Value();
	}
	else {
		PCLKx = RCC_GetPCLK1Value();
	}

	// Check for oversampling rate
	uint32_t usartdiv;
	if (pUSARTx->CR1 & (1 << USART_CR1_OVER8_POS)) {
		// OVER8 bit is set
		usartdiv = ((25 * PCLKx) / (2 * BaudRate));
	}
	else {
		// OVER8 bit is reset. Oversampling by 16
		usartdiv = ((25 * PCLKx) / (4 * BaudRate));
	}

	// Calculate and place mantisa and fractional part
	uint32_t M_part, F_part;
	M_part = usartdiv / 100;
	temp_reg |= (M_part << 4);
	F_part = (usartdiv - (M_part * 100));
	if (pUSARTx->CR1 & (1 << USART_CR1_OVER8_POS)) {
		F_part = (((F_part * 8) + 50) / 100) & ((uint8_t)0x07);
	}
	else {
		F_part = (((F_part * 16) + 50) / 100) & ((uint8_t)0x0F);
	}
	temp_reg |= F_part;

	pUSARTx->BRR = temp_reg;
}

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
	usart_set_baud_rate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baudrate);
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
			// 9-bit format
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PAR_DI) {
				// 9-bit payload. Load DR with 2 bytes
				pData = (uint16_t*) pTxBuffer;
				pUSARTHandle->pUSARTx->DR = (*pData & (uint16_t) 0x01FF);
				pTxBuffer += 2;
			}
			else {
				// 8-bit payload. Hardware adds the 9th parity bit
				pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t) 0xFF);
				pTxBuffer++;
			}
		}
		else {
			// 8-bit payload
			pUSARTHandle->pUSARTx->DR = *pTxBuffer;
			pTxBuffer++;
		}
	}

	// Wait until transmission is completed
	while (USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC) == FLAG_RESET);
}

/**
 * @brief  Receive data using USART protocol
 * @param  pUSARTHandle    Handle structure
 * @param  pRxBuffer       Pointer to reception buffer
 * @param  Len             Number of data frames (words)
 * @retval None
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len) {
	for (uint32_t i = 0; i < Len; i++) {
		// Wait until RXNE is set
		while (USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE) == FLAG_RESET);

		if (pUSARTHandle->USART_Config.USART_WordLen == USART_WORD_9BITS) {
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PAR_DI) {
				*((uint16_t*) pRxBuffer) = (uint16_t) (pUSARTHandle->pUSARTx->DR & 0x01FF);
				pRxBuffer += 2;
			}
			else {
				*pRxBuffer =  (uint8_t) (pUSARTHandle->pUSARTx->DR  & 0xFF);
				pRxBuffer++;
			}
		}
		else {
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PAR_DI) {
				*pRxBuffer = pUSARTHandle->pUSARTx->DR;
			}
			else {
				*pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR & 0x7F);
			}
			pRxBuffer++;
		}
	}
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
