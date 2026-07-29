#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

/* ------------------- PIN CONFIGURATION STRUCTURES ------------------- */

// Configuration structure for a GPIO pin
typedef struct {
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinOType;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinAFMode;
} GPIO_PinConfig_t;

// Handle structure for a GPIO pin
typedef struct {
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

//-------------------------------------------


/* ------------------- SUPPORTED APIs ------------------- */

// PCLK control
void GPIO_PCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

// Initialize and de-initialize
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

// Data read and write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

// Interrupt configuration and handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);

//-------------------------------------------

#endif
