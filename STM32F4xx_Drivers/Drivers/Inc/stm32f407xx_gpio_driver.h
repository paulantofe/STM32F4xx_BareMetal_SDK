/**
 * @file 		stm32f407xx_gpio_driver.h
 * @brief 		Header file for GPIO peripheral support
 *
 * @details     This header file provides the configuration and handle structures for the GPIO peripherals on the MCU. It includes user macros
 *              for pin modes, output types, speeds and pull-up/pull-down resistors, along with the API function prototypes for GPIO
 *              initialization, read/write operations and interrupt handling.
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

/* ------------------- GENERIC MACROS ------------------- */

/**
 * @GPIO_MODES
 */
#define GPIO_MODE_INPUT        0
#define GPIO_MODE_OUTPUT       1
#define GPIO_MODE_ALTFN        2
#define GPIO_MODE_ANALOG       3
#define GPIO_MODE_IT_FT        4
#define GPIO_MODE_IT_RT        5
#define GPIO_MODE_IT_RFT       6

/**
 * @GPIO_OP_TYPES
 */
#define GPIO_OP_TYPE_PP        0
#define GPIO_OP_TYPE_OD        1

/**
 * @GPIO_SPEED
 */
#define GPIO_SPEED_LOW         0
#define GPIO_SPEED_MEDIUM      1
#define GPIO_SPEED_FAST        2
#define GPIO_SPEED_HIGH        3

/**
 * @GPIO_PUPD
 */
#define GPIO_NO_PUPD           0
#define GPIO_PU                1
#define GPIO_PD                2

/**
 * @GPIO_PIN_NUMBER
 */
#define GPIO_PIN_NO_0	 	   0
#define GPIO_PIN_NO_1	 	   1
#define GPIO_PIN_NO_2	 	   2
#define GPIO_PIN_NO_3	 	   3
#define GPIO_PIN_NO_4	 	   4
#define GPIO_PIN_NO_5	 	   5
#define GPIO_PIN_NO_6	 	   6
#define GPIO_PIN_NO_7	 	   7
#define GPIO_PIN_NO_8	 	   8
#define GPIO_PIN_NO_9	 	   9
#define GPIO_PIN_NO_10	 	   10
#define GPIO_PIN_NO_11	 	   11
#define GPIO_PIN_NO_12	 	   12
#define GPIO_PIN_NO_13	 	   13
#define GPIO_PIN_NO_14	 	   14
#define GPIO_PIN_NO_15	 	   15

#define GPIO_BASEADDR_TO_CODE(x)	   ((x == GPIOA) ? 0 : \
                                        (x == GPIOB) ? 1 : \
                                        (x == GPIOC) ? 2 : \
                                        (x == GPIOD) ? 3 : \
                                        (x == GPIOE) ? 4 : \
                                        (x == GPIOF) ? 5 : \
                                        (x == GPIOG) ? 6 : \
                                        (x == GPIOH) ? 7 : \
                                        (x == GPIOI) ? 8 : \
                                        (x == GPIOJ) ? 9 : \
                                        (x == GPIOK) ? 10 : -1)

//-------------------------------------------


/* ------------------- CONFIGURATION STRUCTURES ------------------- */

// Configuration structure for a GPIO pin
typedef struct {
	uint8_t GPIO_PinNumber;         /**< possible values from @GPIO_PIN_NUMBER */
	uint8_t GPIO_PinMode;			/**< possible values from @GPIO_MODES */
	uint8_t GPIO_PinOType;          /**< possible values from @GPIO_OP_TYPES */
	uint8_t GPIO_PinSpeed;          /**< possible values from @GPIO_SPEED */
	uint8_t GPIO_PinPuPdControl;    /**< possible values from @GPIO_PUPD */
	uint8_t GPIO_PinAFMode;
} GPIO_PinConfig_t;

// Handle structure for a GPIO pin
typedef struct {
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

//-------------------------------------------


/* ------------------- SUPPORTED APIs ------------------- */

void GPIO_PCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

//-------------------------------------------

#endif
