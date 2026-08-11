/**
 * @file 		stm32f407xx_spi_driver.h
 * @brief 		Header file for SPI peripheral support
 *
 * @details     This header file provides the configuration and handle structures for the SPI peripherals on the MCU. It includes user macros for
 *              device modes, bus configurations, clock speeds, data frame formats and clock polarity/phase, along with API function prototypes
 *              for SPI initialization, data transmission and interrupt handling.
 */


#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"

/* ------------------- CONFIGURATION STRUCTURES ------------------- */

// Configuration structure for SPI
typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SCLKSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
} SPI_Config_t;

// Handle structure for SPI
typedef struct {
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPI_Config;
} SPI_Handle_t;

//-------------------------------------------


/* ------------------- SUPPORTED APIs ------------------- */

void SPI_PCLKControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t* pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t* pRxBuffer, uint32_t len);

void SPI_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

//-------------------------------------------

#endif
