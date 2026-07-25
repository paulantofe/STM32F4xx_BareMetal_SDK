#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

// addresses of base memory
#define FLASH_BASEADDR         0x08000000UL
#define SRAM1_BASEADDR         0x20000000UL
#define SRAM2_BASEADDR         0x2001C000UL
#define ROM                    0x1FFF0000UL
#define SRAM                   SRAM1_BASEADDR
//-------------------------------------------

// AHBx and APBx bus peripheral base addresses
#define PERIPH_BASEADDR        0x40000000UL
#define APB1PERIPH_BASEADDR    PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR    0x40010000UL
#define AHB1PERIPH_BASEADDR    0x40020000UL
#define AHB2PERIPH_BASEADDR    0x50000000UL
#define AHB3PERIPH_BASEADDR    0xA0000000UL
//-------------------------------------------

#endif
