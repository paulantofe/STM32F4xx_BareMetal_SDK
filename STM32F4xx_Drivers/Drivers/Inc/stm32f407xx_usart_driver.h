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
/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */
/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
