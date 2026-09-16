/**
 * @file 		stm32f407xx_usart_driver.c
 * @brief 		Source file for USART peripheral support
 *
 * @details
 */

#include "stm32f407xx_usart_driver.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */
/* ----------------------------------------------------------------------------------- */


/* -------------------------- APPLICATION CALL BACKS WEAK IMPLEMENTATION -------------------------- */
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

void USART_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);

/* ----------------------------------------------------------------------------------- */
