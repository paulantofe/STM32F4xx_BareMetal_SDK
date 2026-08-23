/**
 * @file 		stm32f407xx_spi_driver.c
 * @brief 		Source file for SPI peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the SPI peripherals on the MCU. It includes the functional
 *              code for peripheral clock control, SPI initialization and de-initialization, data transmission, as well as interrupt
 *              configuration and handling.
 */

#include "stm32f407xx_spi_driver.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */

static void spi_wait_on_flag_timeout(SPI_RegDef_t *pSPIx, uint32_t FlagName, uint8_t Status) {
	uint32_t timeout = 500000;

	while (SPI_GetFlagStatus(pSPIx, FlagName) == Status) {
		timeout--;
		if (timeout == 0) { return; }
	}
}

/* ----------------------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Check status for a specific SPI flag
 * @param  pSPIx	Base address of the SPI peripheral
 * @param  FlagName	Macro of the flag to check @ref SPI_FLAGS
 * @retval FLAG_SET or FLAG_RESET macro
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {
	return (pSPIx->SR & FlagName) ? FLAG_SET : FLAG_RESET;
}

/**
 * @brief  Configure the Internal Slave Select (SSI)
 * @param  pSPIx	Base address of the SPI peripheral
 * @param  EnorDi   ENABLE/DISABLE Macros
 * @retval None
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (pSPIx == NULL) { return; }

	if (EnorDi == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CR1_SSI_POS);
	}
	else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI_POS);
	}
}

/**
 * @brief  Enable/Disable the given SPI peripheral
 * @param  pSPIx	Base address of the SPI peripheral
 * @param  EnorDi   ENABLE/DISABLE Macros
 * @retval None
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (pSPIx == NULL) { return; }

	if (EnorDi == ENABLE) {
		pSPIx->CR1 |= (1 << SPI_CR1_SPE_POS);
	}
	else {
		spi_wait_on_flag_timeout(pSPIx, SPI_TXE_FLAG, FLAG_RESET);

		spi_wait_on_flag_timeout(pSPIx, SPI_BSY_FLAG, FLAG_SET);

		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE_POS);
	}
}

/**
 * @brief  Enable/Disable peripheral clock for a given SPI peripheral
 * @param  pSPIx	Base address of the SPI peripheral
 * @param  EnorDi   ENABLE/DISABLE macros
 * @retval None
 */
void SPI_PClkControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
	if (pSPIx == NULL) { return; }

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
	}
}

/**
 * @brief  Initialize a SPI peripheral with the given settings
 * @param  pSPIHandle	Handle structure with desired settings
 * @retval None
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) {
	if (pSPIHandle == NULL || pSPIHandle->pSPIx == NULL) { return; }

	uint32_t tempReg = 0;

	// Device Mode Configuration
	tempReg |= (pSPIHandle->SPI_Config.SPI_DeviceMode << SPI_CR1_MSTR_POS);

	// Bus Configuration
	if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
		// BIDIMODE should be cleared
		// RXONLY should be cleared
		// As tempReg is initialized with 0, no code is needed
	}
	else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
		// BIDIMODE should be set
		tempReg |= (1 << SPI_CR1_BIDIMODE_POS);

		// RXONLY should be cleared
		// As tempReg is initialized with 0, RXONLY does not need to be cleared
	}
	else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_RXONLY) {
		// BIDIMODE should be cleared
		// As tempReg is initialized with 0, BIDIMODE does not need to be cleared

		// RXONLY should be set
		tempReg |= (1 << SPI_CR1_RXONLY_POS);
	}

	// Baud Rate Configuration
	tempReg |= (pSPIHandle->SPI_Config.SPI_ClkPrescaler << SPI_CR1_BR_POS);

	// DFF Configuration
	tempReg |= (pSPIHandle->SPI_Config.SPI_DFF << SPI_CR1_DFF_POS);

	// CPOL Configuration
	tempReg |= (pSPIHandle->SPI_Config.SPI_CPOL << SPI_CR1_CPOL_POS);

	// CPHA Configuration
    tempReg |= (pSPIHandle->SPI_Config.SPI_CPHA << SPI_CR1_CPHA_POS);

    // SSM Configuration
    tempReg |= (pSPIHandle->SPI_Config.SPI_SSM << SPI_CR1_SSM_POS);

    pSPIHandle->pSPIx->CR1 = tempReg;
}

/**
 * @brief  De-initialize (reset) a SPI peripheral
 * @param  pSPIx	Base address of SPI peripheral
 * @retval None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx) {
	if (pSPIx == NULL) { return; }

	if (pSPIx == SPI1) {
		SPI1_REG_RESET();
	}
	else if (pSPIx == SPI2) {
		SPI2_REG_RESET();
	}
	else if (pSPIx == SPI3) {
		SPI3_REG_RESET();
	}
}

/**
 * @brief  Transmit and/or receive data using SPI protocol
 * @param  pSPIx	   Base address of SPI peripheral
 * @param  pTxBuffer   Pointer to transmission buffer
 * @param  pRxBuffer   Pointer to reception buffer
 * @param  Len         Length of the transmission/reception in bytes
 * @retval None
 * @note               Passing the NULL macro to any of the buffers
 *                     will disable the corresponding communication
 *                     direction. In this case, the API will send/receive
 *                     a dummy value just to generate the required
 *                     clock source.
 */
void SPI_TransmitReceive(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t Len) {
	if (pSPIx == NULL) { return; }

	uint16_t dummy_tx = 0xFFFF;
	uint8_t data_frame = pSPIx->CR1 & (1 << SPI_CR1_DFF_POS);

	while (Len > 0) {
		// Transmission
		spi_wait_on_flag_timeout(pSPIx, SPI_TXE_FLAG, FLAG_RESET);
		if (pTxBuffer != NULL) {
			if (data_frame == 0) {
				pSPIx->DR = *pTxBuffer;
				pTxBuffer++;
			}
			else {
				pSPIx->DR = *((uint16_t*) pTxBuffer);
				pTxBuffer += 2;
			}
		}
		else {
			if (data_frame == 0) { // 8-bit Format
				pSPIx->DR = (uint8_t) dummy_tx;
			}
			else { // 16-bit Format
				pSPIx->DR = dummy_tx;
			}
		}

		// Reception
		spi_wait_on_flag_timeout(pSPIx, SPI_RXNE_FLAG, FLAG_RESET);
		if (pRxBuffer != NULL) { // 8-bit Format
			if (data_frame == 0) {
				*pRxBuffer = pSPIx->DR;
				pRxBuffer++;
			}
			else { // 16-bit Format
				*((uint16_t*) pRxBuffer) = pSPIx->DR;
				pRxBuffer += 2;
			}
		}
		else {
			(void) pSPIx->DR;
		}

		// Length decrement
		if (data_frame == 0) {
			Len--;
		}
		else {
			Len -= 2;
		}
	}
}

/**
 * @brief  Configure an interrupt for SPI peripheral
 * @param  IRQNumber	Number of the interrupt request from IRQn_Type enum
 * @param  EnorDi		ENABLE/DISABLE macros
 * @retval None
 */
void SPI_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi) {
	if (IRQNumber > 81) { return; }

	if (EnorDi == ENABLE) {
		NVIC_ISER_BASEADDR[IRQNumber / 32] = (1 << (IRQNumber % 32));
	}
	else {
		NVIC_ICER_BASEADDR[IRQNumber / 32] = (1 << (IRQNumber % 32));
	}
}

/**
 * @brief  Set interrupt priority for SPI interrupt
 * @param  IRQNumber	Number of the interrupt request from IRQn_Type enum
 * @param  IRQPriority	Priority of the interrupt
 * @retval None
 */
void SPI_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority) {
	if (IRQNumber > 81 || IRQPriority > 15) { return; }

	uint8_t iprx = IRQNumber / 4;
	uint8_t iprxSection = IRQNumber % 4;
	uint8_t shiftAmount = 8 * iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);

	NVIC_IPR_BASEADDR[iprx] &= ~(0xFF << (8 * iprxSection));
	NVIC_IPR_BASEADDR[iprx] |= (IRQPriority << shiftAmount);
}

void SPI_IRQHandling(SPI_Handle_t *pHandle);

/* ----------------------------------------------------------------------------------- */
