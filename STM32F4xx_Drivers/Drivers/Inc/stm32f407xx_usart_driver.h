/**
 * @file 		stm32f407xx_usart_driver.h
 * @brief 		Header file for USART peripheral support
 *
 * @details
 */

#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_

#include "stm32f407xx.h"

/* -------------------------- GENERIC MACROS -------------------------- */
/* -------------------------------------------------------------------- */


/* -------------------------- CONFIGURATION STRUCTURES -------------------------- */

/** @brief Configuration structure for USART */
typedef struct {
	uint32_t USART_Baudrate;
	uint8_t  USART_Mode;
	uint8_t  USART_NoOfStopBits;
	uint8_t  USART_WordLen;
	uint8_t  USART_ParityControl;
	uint8_t  USART_HWFlowControl;
} USART_Config_t;

/** @brief Handle structure for USART */
typedef struct {
	USART_Config_t USART_Config;
	USART_RegDef_t *pUSARTx;
} USART_Handle_t;

/* ------------------------------------------------------------------------------ */


/* -------------------------- APPLICATION CALL BACKS -------------------------- */

void USART_ApplicationCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv);

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

void USART_PClkControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

void USART_SendData(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

void USART_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
