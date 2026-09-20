/**
 * @file 		stm32f407xx_rcc_driver.h
 * @brief 		Header file for RCC peripheral support
 *
 * @details     This header file provides APIs to get the clock source of APB1 and APB2 busses of the MCU.
 */

#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"

/* -------------------------- GENERIC MACROS -------------------------- */

#define HSI_VALUE              16000000U
#define HSE_VALUE              8000000U

/* -------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
