#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

/* ------------------- GENERIC MACROS ------------------- */

#define __vo                   volatile
#define ENABLE                 1
#define DISABLE                0
#define SET                    ENABLE
#define RESET                  DISABLE
#define GPIO_PIN_SET           SET
#define GPIO_PIN_RESET         RESET

//-------------------------------------------



/* ------------------- MEMORY MAPPING ------------------- */

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
#define SPI4_BASEADDR          (APB2PERIPH_BASEADDR + 0x3400UL)
#define SPI5_BASEADDR          (APB2PERIPH_BASEADDR + 0x5000UL)
#define SPI6_BASEADDR          (APB2PERIPH_BASEADDR + 0x5400UL)
#define USART1_BASEADDR        (APB2PERIPH_BASEADDR + 0x1000UL)
#define USART6_BASEADDR        (APB2PERIPH_BASEADDR + 0x1400UL)
#define EXTI_BASEADDR          (APB2PERIPH_BASEADDR + 0x3C00UL)
#define SYSCFGR_BASEADDR       (APB2PERIPH_BASEADDR + 0x3800UL)

//-------------------------------------------


/* ------------------- PERIPHERAL REGISTER DEFINITION STRUCTURES ------------------- */

// GPIO register definition
typedef struct {
	__vo uint32_t MODER;
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];
} GPIO_RegDef_t;

// RCC register definition
typedef struct {
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	uint32_t      RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	uint32_t      RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	uint32_t      RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	uint32_t      RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	uint32_t      RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	uint32_t      RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	uint32_t      RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
} RCC_RegDef_t;

// EXTI register definition
typedef struct {
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
} EXTI_RegDef_t;

// SYSCFG register definition
typedef struct {
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	uint32_t RESERVED0[2];
	__vo uint32_t CMPCR;
} SYSCFG_RegDef_t;

//-------------------------------------------


/* ------------------- PERIPHERAL DEFINITION ------------------- */

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

// APB2 peripherals
#define EXTI                   ((EXTI_RegDef_t*) EXTI_BASEADDR)
#define SYSCFG                 ((SYSCFG_RegDef_t*) SYSCFGR_BASEADDR)

//-------------------------------------------


/* ------------------- CLOCK ENABLE MACROS ------------------- */

#define GPIOA_PCLK_EN()        (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()        (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()        (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()        (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()        (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()        (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()        (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()        (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()        (RCC->AHB1ENR |= (1 << 8))
#define GPIOJ_PCLK_EN()        (RCC->AHB1ENR |= (1 << 9))
#define GPIOK_PCLK_EN()        (RCC->AHB1ENR |= (1 << 10))

#define I2C1_PCLK_EN()         (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()         (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()         (RCC->APB1ENR |= (1 << 23))

#define SPI1_PCLK_EN()         (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()         (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()         (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()         (RCC->APB2ENR |= (1 << 13))
#define SPI5_PCLK_EN()         (RCC->APB2ENR |= (1 << 20))
#define SPI6_PCLK_EN()         (RCC->APB2ENR |= (1 << 21))

#define UART4_PCLK_EN()        (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()        (RCC->APB1ENR |= (1 << 20))
#define UART7_PCLK_EN()        (RCC->APB1ENR |= (1 << 30))
#define UART8_PCLK_EN()        (RCC->APB1ENR |= (1 << 31))

#define USART1_PCLK_EN()       (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()       (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()       (RCC->APB1ENR |= (1 << 18))
#define USART6_PCLK_EN()       (RCC->APB2ENR |= (1 << 5))

#define SYSCFGR_PCLK_EN()      (RCC->APB2ENR |= (1 << 14))

//-------------------------------------------


/* ------------------- CLOCK DISABLE MACROS ------------------- */

#define GPIOA_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 8))
#define GPIOJ_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 9))
#define GPIOK_PCLK_DI()        (RCC->AHB1ENR &= ~(1 << 10))

#define I2C1_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 23))

#define SPI1_PCLK_DI()         (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()         (RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()         (RCC->APB2ENR &= ~(1 << 13))
#define SPI5_PCLK_DI()         (RCC->APB2ENR &= ~(1 << 20))
#define SPI6_PCLK_DI()         (RCC->APB2ENR &= ~(1 << 21))

#define UART4_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 20))
#define UART7_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 30))
#define UART8_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 31))

#define USART1_PCLK_DI()       (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()       (RCC->APB1ENR &= ~(1 << 18))
#define USART6_PCLK_DI()       (RCC->APB2ENR &= ~(1 << 5))

#define SYSCFGR_PCLK_DI()      (RCC->APB2ENR &= ~(1 << 14))

//-------------------------------------------


/* ------------------- GPIO RESET MACROS ------------------- */

#define GPIOA_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 0); RCC->AHB1RSTR &= ~(1 << 0); } while (0)
#define GPIOB_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 1); RCC->AHB1RSTR &= ~(1 << 1); } while (0)
#define GPIOC_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 2); RCC->AHB1RSTR &= ~(1 << 2); } while (0)
#define GPIOD_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 3); RCC->AHB1RSTR &= ~(1 << 3); } while (0)
#define GPIOE_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 4); RCC->AHB1RSTR &= ~(1 << 4); } while (0)
#define GPIOF_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 5); RCC->AHB1RSTR &= ~(1 << 5); } while (0)
#define GPIOG_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 6); RCC->AHB1RSTR &= ~(1 << 6); } while (0)
#define GPIOH_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 7); RCC->AHB1RSTR &= ~(1 << 7); } while (0)
#define GPIOI_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 8); RCC->AHB1RSTR &= ~(1 << 8); } while (0)
#define GPIOJ_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 9); RCC->AHB1RSTR &= ~(1 << 9); } while (0)
#define GPIOK_REG_RESET()      do { RCC->AHB1RSTR |= (1 << 10); RCC->AHB1RSTR &= ~(1 << 10); } while (0)

//-------------------------------------------

#endif
