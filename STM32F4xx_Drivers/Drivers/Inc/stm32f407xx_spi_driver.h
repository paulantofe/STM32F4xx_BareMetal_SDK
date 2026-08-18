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

/* -------------------------- GENERIC MACROS -------------------------- */

/**
 * @defgroup SPI_DEVICE_MODES  Device Modes
 * @{
 */
#define SPI_DEVICE_MODE_SLAVE  0
#define SPI_DEVICE_MODE_MASTER 1
/** @} */

/**
 * @defgroup SPI_BUS_CONFIG    Bus Configuration
 * @{
 */
#define SPI_BUS_CONFIG_FD      1      // full duplex
#define SPI_BUS_CONFIG_HD      2      // half duplex
#define SPI_BUS_CONFIG_RXONLY  3      // simplex Rx only
/** @} */

/**
 * @defgroup SPI_SCLK_SPEED    Serial Clock Speed
 * @{
 */
#define SPI_SCLK_SPEED_DIV2    0
#define SPI_SCLK_SPEED_DIV4    1
#define SPI_SCLK_SPEED_DIV8    2
#define SPI_SCLK_SPEED_DIV16   3
#define SPI_SCLK_SPEED_DIV32   4
#define SPI_SCLK_SPEED_DIV64   5
#define SPI_SCLK_SPEED_DIV128  6
#define SPI_SCLK_SPEED_DIV256  7
/** @} */

/**
 * @defgroup SPI_DFF           Data Frame Format
 * @{
 */
#define SPI_DFF_8_BITS         0
#define SPI_DFF_16_BITS        1
/** @} */

/**
 * @defgroup SPI_CPOL          Clock Polarity
 * @{
 */
#define SPI_CPOL_LOW           0
#define SPI_CPOL_HIGH          1
/** @} */

/**
 * @defgroup SPI_CPHA          Clock Phase
 * @{
 */
#define SPI_CPHA_FIRST_EDGE    0
#define SPI_CPHA_SECOND_EDGE   1
/** @} */

/**
 * @defgroup SPI_SSM           Software Slave Management
 * @{
 */
#define SPI_SSM_DI             0
#define SPI_SSM_EN             1
/** @} */

/**
 * @defgroup SPI_FLAGS         Status Flags
 * @{
 */
#define SPI_TXE_FLAG           (1 << SPI_SR_TXE_POS)        // Tx Buffer Empty Flag
#define SPI_RXNE_FLAG          (1 << SPI_SR_RXNE_POS)       // Rx Buffer Not Empty Flag
#define SPI_BSY_FLAG           (1 << SPI_SR_BSY_POS)        // Busy In Communication Flag
/** @} */

/* -------------------------------------------------------------------- */


/* -------------------------- CONFIGURATION STRUCTURES -------------------------- */

/** @brief Configuration structure for SPI */
typedef struct {
	uint8_t SPI_DeviceMode;    /**< possible values from @ref SPI_DEVICE_MODES */
	uint8_t SPI_BusConfig;     /**< possible values from @ref SPI_BUS_CONFIG   */
	uint8_t SPI_SCLKSpeed;     /**< possible values from @ref SPI_SCLK_SPEED   */
	uint8_t SPI_DFF;           /**< possible values from @ref SPI_DFF          */
	uint8_t SPI_CPOL;          /**< possible values from @ref SPI_CPOL         */
	uint8_t SPI_CPHA;          /**< possible values from @ref SPI_CPHA         */
	uint8_t SPI_SSM;           /**< possible values from @ref SPI_SSM          */
} SPI_Config_t;

/** @brief Handle structure for SPI */
typedef struct {
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPI_Config;
} SPI_Handle_t;

/* ------------------------------------------------------------------------------ */


/* -------------------------- SUPPORTED APIs (for more information check the function definitions) -------------------------- */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t pSPIx, uint32_t FlagName);

void SPI_PCLKControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t* pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t* pRxBuffer, uint32_t len);

void SPI_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/* -------------------------------------------------------------------------------------------------------------------------- */

#endif
