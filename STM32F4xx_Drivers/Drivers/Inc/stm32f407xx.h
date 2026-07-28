#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo                   volatile

/* ------------------- MEMORY MAPPING ------------------- */

// Memory base addresses
#define FLASH_BASEADDR         0x08000000UL
#define SRAM1_BASEADDR         0x20000000UL
#define SRAM2_BASEADDR         0x2001C000UL
#define ROM                    0x1FFF0000UL
#define SRAM                   SRAM1_BASEADDR
//-------------------------------------------

// AHBx and APBx bus base addresses
#define PERIPH_BASEADDR        0x40000000UL
#define APB1PERIPH_BASEADDR    PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR    0x40010000UL
#define AHB1PERIPH_BASEADDR    0x40020000UL
#define AHB2PERIPH_BASEADDR    0x50000000UL
#define AHB3PERIPH_BASEADDR    0xA0000000UL
//-------------------------------------------

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
//-------------------------------------------

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
//-------------------------------------------

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

//GPIO register definition
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

//RCC register definition
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
//-------------------------------------------


/* ------------------- PERIPHERAL DEFINITION ------------------- */

//GPIO
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

//RCC
#define RCC                    ((RCC_RegDef_t*) RCC_BASEADDR)
//-------------------------------------------

#endif
