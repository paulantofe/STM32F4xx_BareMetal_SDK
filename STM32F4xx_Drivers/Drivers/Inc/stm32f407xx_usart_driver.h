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

/**
 * @defgroup USART_MODES          Mode Of USART Peripheral
 * @{
 */
#define USART_MODE_TX_ONLY         0
#define USART_MODE_RX_ONLY         1
#define USART_MODE_FD              2
/** @} */

/**
 * @defgroup USART_BAUDRATE        Standard Baud Rates of Peripheral (bps)
 * @{
 */
#define USART_STD_BAUD_1200        1200
#define USART_STD_BAUD_2400        2400
#define USART_STD_BAUD_9600        9600
#define USART_STD_BAUD_19200       19200
#define USART_STD_BAUD_38400       38400
#define USART_STD_BAUD_57600       57600
#define USART_STD_BAUD_115200      115200
#define USART_STD_BAUD_230400      230400
#define USART_STD_BAUD_460800      460800
#define USART_STD_BAUD_921600      921600
#define USART_STD_BAUD_2000000     2000000
#define USART_STD_BAUD_3000000     3000000
/** @} */

/**
 * @defgroup USART_PAR_CTLR        Parity Control
 * @{
 */
#define USART_PAR_DI               0
#define USART_PAR_EN_EVEN          1
#define USART_PAR_EN_ODD           2
/** @} */

/**
 * @defgroup USART_WORD_LEN        Word Length
 * @{
 */
#define USART_WORD_8BITS           0
#define USART_WORD_9BITS           1
/** @} */

/**
 * @defgroup USART_STOP_BITS       Number of Stop Bits
 * @{
 */
#define USART_STOP_BITS_1          0
#define USART_STOP_BITS_0_5        1
#define USART_STOP_BITS_2          2
#define USART_STOP_BITS_1_5        3
/** @} */

/**
 * @defgroup USART_HW_FLOW_CTRL    Hardware Flow Control
 * @{
 */
#define USART_HW_FLOW_NONE         0
#define USART_HW_FLOW_CTS          1
#define USART_HW_FLOW_RTS          2
#define USART_HW_FLOW_CTS_RTS      3
/** @} */

/**
 * @defgroup USART_FLAGS           Status Flags
 * @{
 */
#define USART_FLAG_PE              (1 << USART_SR_PE_POS)    // Parity Error
#define USART_FLAG_FE              (1 << USART_SR_FE_POS)    // Framing Error
#define USART_FLAG_NF              (1 << USART_SR_NF_POS)    // Noise Flag
#define USART_FLAG_ORE             (1 << USART_SR_ORE_POS)   // Overrun Error
#define USART_FLAG_IDLE            (1 << USART_SR_IDLE_POS)  // IDLE Line Detected
#define USART_FLAG_RXNE            (1 << USART_SR_RXNE_POS)  // Read Data Register Not Empty
#define USART_FLAG_TC              (1 << USART_SR_TC_POS)    // Transmission Complete
#define USART_FLAG_TXE             (1 << USART_SR_TXE_POS)   // Transmit Data Register Empty
#define USART_FLAG_LBD             (1 << USART_SR_LBD_POS)   // LIN Break Detection
#define USART_FLAG_CTS             (1 << USART_SR_CTS_POS)   // CTS Flag
/** @} */

/* -------------------------------------------------------------------- */


/* -------------------------- CONFIGURATION STRUCTURES -------------------------- */

/** @brief Configuration structure for USART */
typedef struct {
	uint32_t USART_Baudrate;            /**< possible values from @ref USART_BAUDRATE      */
	uint8_t  USART_Mode;                /**< possible values from @ref USART_MODES         */
	uint8_t  USART_NoOfStopBits;        /**< possible values from @ref USART_STOP_BITS     */
	uint8_t  USART_WordLen;             /**< possible values from @ref USART_WORD_LEN      */
	uint8_t  USART_ParityControl;       /**< possible values from @ref USART_PAR_CTLR      */
	uint8_t  USART_HWFlowControl;       /**< possible values from @ref USART_HW_FLOW_CTRL  */
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
