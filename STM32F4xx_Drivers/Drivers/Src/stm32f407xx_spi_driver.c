/**
 * @file 		stm32f407xx_spi_driver.c
 * @brief 		Source file for SPI peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the SPI peripherals on the MCU. It includes the functional
 *              code for peripheral clock control, SPI initialization and de-initialization, data transmission, as well as interrupt
 *              configuration and handling.
 */

#include "stm32f407xx_spi_driver.h"

/* ------------------- SUPPORTED APIs IMPLEMENTATION ------------------- */

void SPI_PCLKControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_EN();
		}
		else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		}
		else if (pSPIx == SPI3) {
			SPI3_PCLK_EN();
		}
		else if (pSPIx == SPI4) {
			SPI4_PCLK_EN();
		}
		else if (pSPIx == SPI5) {
			SPI5_PCLK_EN();
		}
		else if (pSPIx == SPI6) {
			SPI6_PCLK_EN();
		}
	} else {
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		}
		else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		}
		else if (pSPIx == SPI3) {
			SPI3_PCLK_DI();
		}
		else if (pSPIx == SPI4) {
			SPI4_PCLK_DI();
		}
		else if (pSPIx == SPI5) {
			SPI5_PCLK_DI();
		}
		else if (pSPIx == SPI6) {
			SPI6_PCLK_DI();
		}
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx) {
	if (pSPIx == SPI1) {
		SPI1_REG_RESET();
	}
	else if (pSPIx == SPI2) {
		SPI2_REG_RESET();
	}
	else if (pSPIx == SPI3) {
		SPI3_REG_RESET();
	}
	else if (pSPIx == SPI4) {
		SPI4_REG_RESET();
	}
	else if (pSPIx == SPI5) {
		SPI5_REG_RESET();
	}
	else if (pSPIx == SPI6) {
		SPI6_REG_RESET();
	}
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t* pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t* pRxBuffer, uint32_t len);

void SPI_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

//-------------------------------------------
