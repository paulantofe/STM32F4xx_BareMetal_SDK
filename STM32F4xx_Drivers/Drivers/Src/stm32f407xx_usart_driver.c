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
 * @param  FlagName Macro of the flag to check @ref USART_FLAGS
 * @retval FLAG_SET or FLAG_RESET macro
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName) {
	if (pUSARTx == NULL) { return FLAG_RESET; }

	return (pUSARTx->SR & FlagName) ? FLAG_SET : FLAG_RESET;
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

void USART_PClkControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

void USART_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);

/* ----------------------------------------------------------------------------------- */
