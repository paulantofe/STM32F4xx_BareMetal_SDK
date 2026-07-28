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
void GPIO_PCLKControl(void);

// Initialize and de-initialize
void GPIO_Init(void);
void GPIO_DeInit(void);

// Data read and write
void GPIO_ReadFromInputPin(void);
void GPIO_ReadFromInputPort(void);
void GPIO_WriteToOutputPin(void);
void GPIO_WriteToOutputPort(void);
void GPIO_ToggleOutputPin(void);

// Interrupt configuration and handling
void GPIO_IRQConfig(void);
void GPIO_IRQHandling(void);

//-------------------------------------------

#endif
