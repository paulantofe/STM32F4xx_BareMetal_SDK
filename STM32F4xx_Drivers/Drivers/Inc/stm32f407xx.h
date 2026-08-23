/**
 * @file 		stm32f407xx.h
 * @brief 		Generic header file for stm32f407xx microcontroller
 *
 * @details     This header file defines generic macros, maps the memory and peripheral addresses and provides register definition structures
 *              for core peripherals (GPIO, RCC, SPI, EXTI, SYSCFG, I2C, UART, USART). Additionally, it contains helper macros for peripheral
 *              clock management, register resets and IRQ definitions for the NVIC.
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

/* -------------------------- GENERIC MACROS -------------------------- */

#define __vo                   volatile
#define __weak                 __attribute__((weak))
#define ENABLE                 1
#define DISABLE                0
#define SET                    ENABLE
#define RESET                  DISABLE
#define GPIO_PIN_SET           SET
#define GPIO_PIN_RESET         RESET
#define FLAG_SET               SET
#define FLAG_RESET             RESET
#define NULL                   ((void*) 0)
#define NO_PR_BITS_IMPLEMENTED 4

/* -------------------------------------------------------------------- */


/* -------------------------- PROCESSOR SPECIFIC DETAILS (ARM Cortex-M4) -------------------------- */

#define NVIC_ISER_BASEADDR     ((__vo uint32_t*) 0xE000E100UL)
#define NVIC_ICER_BASEADDR     ((__vo uint32_t*) 0xE000E180UL)
#define NVIC_IPR_BASEADDR      ((__vo uint32_t*) 0xE000E400UL)

/* ------------------------------------------------------------------------------------------------ */


/* -------------------------- MEMORY MAPPING -------------------------- */

// Memory base addresses
#define FLASH_BASEADDR         0x08000000UL
#define SRAM1_BASEADDR         0x20000000UL
#define SRAM2_BASEADDR         0x2001C000UL
#define ROM                    0x1FFF0000UL
#define SRAM                   SRAM1_BASEADDR

// AHBx and APBx bus base addresses
#define PERIPH_BASEADDR        0x40000000UL
#define APB1PERIPH_BASEADDR    PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR    0x40010000UL
#define AHB1PERIPH_BASEADDR    0x40020000UL
#define AHB2PERIPH_BASEADDR    0x50000000UL
#define AHB3PERIPH_BASEADDR    0xA0000000UL

// AHB1 peripherals base addresses
#define GPIOA_BASEADDR         (AHB1PERIPH_BASEADDR + 0x0000UL)
#define GPIOB_BASEADDR         (AHB1PERIPH_BASEADDR + 0x0400UL)
#define GPIOC_BASEADDR         (AHB1PERIPH_BASEADDR + 0x0800UL)
#define GPIOD_BASEADDR         (AHB1PERIPH_BASEADDR + 0x0C00UL)
#define GPIOE_BASEADDR         (AHB1PERIPH_BASEADDR + 0x1000UL)
#define GPIOF_BASEADDR         (AHB1PERIPH_BASEADDR + 0x1400UL)
#define GPIOG_BASEADDR         (AHB1PERIPH_BASEADDR + 0x1800UL)
#define GPIOH_BASEADDR         (AHB1PERIPH_BASEADDR + 0x1C00UL)
#define GPIOI_BASEADDR         (AHB1PERIPH_BASEADDR + 0x2000UL)
#define GPIOJ_BASEADDR         (AHB1PERIPH_BASEADDR + 0x2400UL)
#define GPIOK_BASEADDR         (AHB1PERIPH_BASEADDR + 0x2800UL)
#define RCC_BASEADDR           (AHB1PERIPH_BASEADDR + 0x3800UL)

// APB1 peripherals base addresses
#define I2C1_BASEADDR          (APB1PERIPH_BASEADDR + 0x5400UL)
#define I2C2_BASEADDR          (APB1PERIPH_BASEADDR + 0x5800UL)
#define I2C3_BASEADDR          (APB1PERIPH_BASEADDR + 0x5C00UL)
#define SPI2_BASEADDR          (APB1PERIPH_BASEADDR + 0x3800UL)
#define SPI3_BASEADDR          (APB1PERIPH_BASEADDR + 0x3C00UL)
#define USART2_BASEADDR        (APB1PERIPH_BASEADDR + 0x4400UL)
#define USART3_BASEADDR        (APB1PERIPH_BASEADDR + 0x4800UL)
#define UART4_BASEADDR         (APB1PERIPH_BASEADDR + 0x4C00UL)
#define UART5_BASEADDR         (APB1PERIPH_BASEADDR + 0x5000UL)
#define UART7_BASEADDR         (APB1PERIPH_BASEADDR + 0x7800UL)
#define UART8_BASEADDR         (APB1PERIPH_BASEADDR + 0x7C00UL)

// APB2 peripherals base addresses
#define SPI1_BASEADDR          (APB2PERIPH_BASEADDR + 0x3000UL)
#define USART1_BASEADDR        (APB2PERIPH_BASEADDR + 0x1000UL)
#define USART6_BASEADDR        (APB2PERIPH_BASEADDR + 0x1400UL)
#define EXTI_BASEADDR          (APB2PERIPH_BASEADDR + 0x3C00UL)
#define SYSCFGR_BASEADDR       (APB2PERIPH_BASEADDR + 0x3800UL)

/* -------------------------------------------------------------------- */


/* -------------------------- PERIPHERAL REGISTER DEFINITION STRUCTURES -------------------------- */

/** @brief GPIO register definition */
typedef struct {
	__vo uint32_t MODER;       /**< Mode Register                                offset 0x00 */
	__vo uint32_t OTYPER;      /**< Output Type Register                         offset 0x04 */
	__vo uint32_t OSPEEDR;     /**< Output Speed Register                        offset 0x08 */
	__vo uint32_t PUPDR;       /**< Pull-Up/Pull-Down Register                   offset 0x0C */
	__vo uint32_t IDR;         /**< Input Data Register                          offset 0x10 */
	__vo uint32_t ODR;         /**< Output Data Register                         offset 0x14 */
	__vo uint32_t BSRR;        /**< Bit Set/Reset Register                       offset 0x18 */
	__vo uint32_t LCKR;        /**< Configuration Lock Register                  offset 0x1C */
	__vo uint32_t AFR[2];      /**< Alternate Function Registers                 offset 0x20
	                                @note AFR[0]: AF Low Register                offset 0x20
	                                      AFR[1]: AF High Register               offset 0x24 */
} GPIO_RegDef_t;

/** @brief RCC register definition */
typedef struct {
	__vo uint32_t CR;          /**< Clock Control Register                       offset 0x00  */
	__vo uint32_t PLLCFGR;     /**< PLL Configuration Register                   offset 0x04  */
	__vo uint32_t CFGR; 	   /**< Clock Configuration Register                 offset 0x08  */
	__vo uint32_t CIR;         /**< Clock Interrupt Register                     offset 0x0C  */
	__vo uint32_t AHB1RSTR;    /**< AHB1 Peripheral Reset Register               offset 0x10  */
	__vo uint32_t AHB2RSTR;    /**< AHB2 Peripheral Reset Register               offset 0x14  */
	__vo uint32_t AHB3RSTR;    /**< AHB3 Peripheral Reset Register               offset 0x18  */
	uint32_t RESERVED0;        /**< RESERVED MEMORY                              offset 0x1C  */
	__vo uint32_t APB1RSTR;	   /**< APB1 Peripheral Reset Register               offset 0x20  */
	__vo uint32_t APB2RSTR;    /**< APB2 Peripheral Reset Register               offset 0x24  */
	uint32_t RESERVED1[2]; 	   /**< RESERVED MEMORY                              offset 0x28  */
	__vo uint32_t AHB1ENR; 	   /**< AHB1 Peripheral Clock Enable Register        offset 0x30  */
	__vo uint32_t AHB2ENR; 	   /**< AHB2 Peripheral Clock Enable Register        offset 0x34  */
	__vo uint32_t AHB3ENR;     /**< AHB3 Peripheral Clock Enable Register        offset 0x38  */
	uint32_t RESERVED2;        /**< RESERVED MEMORY                              offset 0x3C  */
	__vo uint32_t APB1ENR;     /**< APB1 Peripheral Clock Enable Register        offset 0x40  */
	__vo uint32_t APB2ENR;     /**< APB2 Peripheral Clock Enable Register        offset 0x44  */
	uint32_t RESERVED3[2];     /**< RESERVED MEMORY                              offset 0x48  */
	__vo uint32_t AHB1LPENR;   /**< AHB1 PCLK Enable Low Power Mode Register     offset 0x50  */
	__vo uint32_t AHB2LPENR;   /**< AHB2 PCLK Enable Low Power Mode Register     offset 0x54  */
	__vo uint32_t AHB3LPENR;   /**< AHB3 PCLK Enable Low Power Mode Register     offset 0x58  */
	uint32_t RESERVED4;        /**< RESERVED MEMORY                              offset 0x5C  */
	__vo uint32_t APB1LPENR;   /**< APB1 PCLK Enable Low Power Mode Register     offset 0x60  */
	__vo uint32_t APB2LPENR;   /**< APB2 PCLK Enable Low Power Mode Register     offset 0x64  */
	uint32_t RESERVED5[2];     /**< RESERVED MEMORY                              offset 0x68  */
	__vo uint32_t BDCR;        /**< Backup Domain Control Register               offset 0x70  */
	__vo uint32_t CSR;         /**< Clock Control and Status Register            offset 0x74  */
	uint32_t RESERVED6[2];     /**< RESERVED MEMORY                              offset 0x78  */
	__vo uint32_t SSCGR;       /**< Spread Spectrum CLK Generation Register      offset 0x80  */
	__vo uint32_t PLLI2SCFGR;  /**< PLLI2S Configuration Register                offset 0x84  */
} RCC_RegDef_t;

/** @brief EXTI register definition */
typedef struct {
	__vo uint32_t IMR;         /**< Interrupt Mask Register                      offset 0x00 */
	__vo uint32_t EMR;         /**< Event Mask Register                          offset 0x04 */
	__vo uint32_t RTSR;        /**< Rising Trigger Selection Register            offset 0x08 */
	__vo uint32_t FTSR;        /**< Falling Trigger Selection Register           offset 0x0C */
	__vo uint32_t SWIER;       /**< Software Interrupt Event Register            offset 0x10 */
	__vo uint32_t PR;          /**< Pending Register                             offset 0x14 */
} EXTI_RegDef_t;

/** @brief SYSCFG register definition */
typedef struct {
	__vo uint32_t MEMRMP;      /**< Memory Remap Register                        offset 0x00 */
	__vo uint32_t PMC;         /**< Peripheral Mode Configuration Register       offset 0x04 */
	__vo uint32_t EXTICR[4];   /**< External Interrupt Configuration Register    offset 0x08 */
	uint32_t RESERVED0[2];     /**< RESERVED MEMORY                              offset 0x18 */
	__vo uint32_t CMPCR;       /**< Compensation Cell Control Register           offset 0x20 */
} SYSCFG_RegDef_t;

/** @brief SPI register definition */
typedef struct {
	__vo uint32_t CR1;         /**< Control Register 1                           offset 0x00 */
	__vo uint32_t CR2;         /**< Control Register 2                           offset 0x04 */
	__vo uint32_t SR;          /**< Status Register                              offset 0x08 */
	__vo uint32_t DR;          /**< Data Register                                offset 0x0C */
	__vo uint32_t CRCPR;       /**< CRC Polynomial Register                      offset 0x10 */
	__vo uint32_t RXCRCR;      /**< Rx CRC Register                              offset 0x14 */
	__vo uint32_t TXCRCR;      /**< Tx CRC Register                              offset 0x18 */
	__vo uint32_t I2SCFGR;     /**< I2S Configuration Register                   offset 0x1C */
	__vo uint32_t I2SPR;       /**< I2S Prescaler Register                       offset 0x20 */
} SPI_RegDef_t;

/* ----------------------------------------------------------------------------------------------- */


/* -------------------------- BIT POSITION DEFINITIONS -------------------------- */

/* ---------- GPIO Peripheral ---------- */

// Bit position calculation macros for GPIO pins (x = Pin Number: 0 to 15)
#define GPIO_MODER_PIN_POS(x)   ((x) * 2)
#define GPIO_OTYPER_PIN_POS(x)  (x)
#define GPIO_OSPEEDR_PIN_POS(x) ((x) * 2)
#define GPIO_PUPDR_PIN_POS(x)   ((x) * 2)
#define GPIO_IDR_PIN_POS(x)     (x)
#define GPIO_ODR_PIN_POS(x)     (x)

// BSRR (Bit Set/Reset Register) positions
#define GPIO_BSRR_SET_POS(x)    (x)
#define GPIO_BSRR_RESET_POS(x)  ((x) + 16)

// AFR (Alternate Function Registers) positions
#define GPIO_AFRL_PIN_POS(x)    ((x) * 4)          // For pins 0-7
#define GPIO_AFRH_PIN_POS(x)    (((x) % 8) * 4)    // For pins 8-15

/* ------------------------------------- */

/* ---------- SPI Peripheral ---------- */

// Bit position definitions for SPI_CR1
#define SPI_CR1_CPHA_POS       0
#define SPI_CR1_CPOL_POS       1
#define SPI_CR1_MSTR_POS       2
#define SPI_CR1_BR_POS         3
#define SPI_CR1_SPE_POS        6
#define SPI_CR1_LSBFIRST_POS   7
#define SPI_CR1_SSI_POS        8
#define SPI_CR1_SSM_POS        9
#define SPI_CR1_RXONLY_POS     10
#define SPI_CR1_DFF_POS        11
#define SPI_CR1_CRCNEXT_POS    12
#define SPI_CR1_CRCEN_POS      13
#define SPI_CR1_BIDIOE_POS     14
#define SPI_CR1_BIDIMODE_POS   15

// Bit position definitions for SPI_CR2
#define SPI_CR2_RXDMAEN_POS      0
#define SPI_CR2_TXDMAEN_POS      1
#define SPI_CR2_SSOE_POS         2
#define SPI_CR2_FRF_POS          4
#define SPI_CR2_ERRIE_POS        5
#define SPI_CR2_RXNEIE_POS       6
#define SPI_CR2_TXEIE_POS        7

// Bit position definitions for SPI_SR
#define SPI_SR_RXNE_POS          0
#define SPI_SR_TXE_POS           1
#define SPI_SR_CHSIDE_POS        2
#define SPI_SR_UDR_POS           3
#define SPI_SR_CRCERR_POS        4
#define SPI_SR_MODF_POS          5
#define SPI_SR_OVR_POS           6
#define SPI_SR_BSY_POS           7
#define SPI_SR_FRE_POS           8

/* ------------------------------------ */

/* ----------------------------------------------------------------------------- */


/* -------------------------- PERIPHERAL DEFINITIONS -------------------------- */

// AHB1 peripherals
#define GPIOA                  ((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB                  ((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC                  ((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD                  ((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE                  ((GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOF                  ((GPIO_RegDef_t*) GPIOF_BASEADDR)
#define GPIOG                  ((GPIO_RegDef_t*) GPIOG_BASEADDR)
#define GPIOH                  ((GPIO_RegDef_t*) GPIOH_BASEADDR)
#define GPIOI                  ((GPIO_RegDef_t*) GPIOI_BASEADDR)
#define GPIOJ                  ((GPIO_RegDef_t*) GPIOJ_BASEADDR)
#define GPIOK                  ((GPIO_RegDef_t*) GPIOK_BASEADDR)
#define RCC                    ((RCC_RegDef_t*) RCC_BASEADDR)

// APB1 peripherals
#define SPI2                   ((SPI_RegDef_t*) SPI2_BASEADDR)
#define SPI3                   ((SPI_RegDef_t*) SPI3_BASEADDR)

// APB2 peripherals
#define EXTI                   ((EXTI_RegDef_t*) EXTI_BASEADDR)
#define SYSCFG                 ((SYSCFG_RegDef_t*) SYSCFGR_BASEADDR)
#define SPI1                   ((SPI_RegDef_t*) SPI1_BASEADDR)

/* --------------------------------------------------------------------------- */


/* -------------------------- CLOCK ENABLE MACROS -------------------------- */

#define I2C1_PCLK_EN()         (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()         (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()         (RCC->APB1ENR |= (1 << 23))

#define SPI1_PCLK_EN()         (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()         (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()         (RCC->APB1ENR |= (1 << 15))

#define UART4_PCLK_EN()        (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()        (RCC->APB1ENR |= (1 << 20))
#define UART7_PCLK_EN()        (RCC->APB1ENR |= (1 << 30))
#define UART8_PCLK_EN()        (RCC->APB1ENR |= (1 << 31))

#define USART1_PCLK_EN()       (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()       (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()       (RCC->APB1ENR |= (1 << 18))
#define USART6_PCLK_EN()       (RCC->APB2ENR |= (1 << 5))

#define SYSCFG_PCLK_EN()       (RCC->APB2ENR |= (1 << 14))

/* ------------------------------------------------------------------------- */


/* -------------------------- CLOCK DISABLE MACROS -------------------------- */

#define I2C1_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 23))

#define SPI1_PCLK_DI()         (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 15))

#define UART4_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 20))
#define UART7_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 30))
#define UART8_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 31))

#define USART1_PCLK_DI()       (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 18))
#define USART6_PCLK_DI()       (RCC->APB2ENR &= ~(1 << 5))

#define SYSCFG_PCLK_DI()       (RCC->APB2ENR &= ~(1 << 14))

/* -------------------------------------------------------------------------- */


/* -------------------------- RESET MACROS -------------------------- */

#define SPI1_REG_RESET()       do { RCC->APB2RSTR |= (1 << 12); RCC->APB2RSTR &= ~(1 << 12); } while (0)
#define SPI2_REG_RESET()       do { RCC->APB1RSTR |= (1 << 14); RCC->APB1RSTR &= ~(1 << 14); } while (0)
#define SPI3_REG_RESET()       do { RCC->APB1RSTR |= (1 << 15); RCC->APB1RSTR &= ~(1 << 15); } while (0)

/* ------------------------------------------------------------------ */


/* -------------------------- Interrupt Request Numbers -------------------------- */

typedef enum {
	EXTI0_IRQn      = 6,
	EXTI1_IRQn      = 7,
	EXTI2_IRQn      = 8,
	EXTI3_IRQn      = 9,
	EXTI4_IRQn      = 10,
	EXTI9_5_IRQn    = 23,
	EXTI15_10_IRQn  = 40,
	SPI1_IRQn       = 35,
	SPI2_IRQn       = 36,
	SPI3_IRQn       = 51
} IRQn_Type;

/* ------------------------------------------------------------------------------- */


/* -------------------------- Interrupt Request Priority Numbers -------------------------- */

#define NVIC_IRQ_PR0           0
#define NVIC_IRQ_PR1           1
#define NVIC_IRQ_PR2           2
#define NVIC_IRQ_PR3           3
#define NVIC_IRQ_PR4           4
#define NVIC_IRQ_PR5           5
#define NVIC_IRQ_PR6           6
#define NVIC_IRQ_PR7           7
#define NVIC_IRQ_PR8           8
#define NVIC_IRQ_PR9           9
#define NVIC_IRQ_PR10          10
#define NVIC_IRQ_PR11          11
#define NVIC_IRQ_PR12          12
#define NVIC_IRQ_PR13          13
#define NVIC_IRQ_PR14          14
#define NVIC_IRQ_PR15          15

/* ---------------------------------------------------------------------------------------- */

#endif
