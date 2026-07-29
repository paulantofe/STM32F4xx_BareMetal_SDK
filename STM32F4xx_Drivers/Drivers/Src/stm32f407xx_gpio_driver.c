#include "stm32f407xx_gpio_driver.h"

// ------------------- DOCUMENTATION -------------------

/*
 * function:               GPIO_PCLKControl
 * brief:                  This function enables/disables peripheral clock for the given GPIO port
 * parameter[in]:          Base address of the GPIO peripheral
 * parameter[in]:          ENABLE or DISABLE macros
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_Init
 * brief:                  This function initializes a GPIO pin with the given settings
 * parameter[in]:          GPIO handle structure with the desired settings
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_DeInit
 * brief:                  This function resets a GPIO pin
 * parameter[in]:          Base address of the GPIO peripheral
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_ReadFromInputPin
 * brief:
 * parameter[in]:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_ReadFromInputPort
 * brief:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_WriteToOutputPin
 * brief:
 * parameter[in]:
 * parameter[in]:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_WriteToOutputPort
 * brief:
 * parameter[in]:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_ToggleOutputPin
 * brief:
 * parameter[in]:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_IRQConfig
 * brief:
 * parameter[in]:
 * parameter[in]:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
 * function:               GPIO_IRQHandling
 * brief:
 * parameter[in]:
 * return:                 None
 * note:                   None
 * ------------------------------------------
*/

//-------------------------------------------


/* ------------------- SUPPORTED APIs IMPLEMENTATION ------------------- */

// PCLK control
void GPIO_PCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {

}

// Initialize and de-initialize
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {

}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {

}

// Data read and write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {

}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {

}
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value) {

}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value) {

}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {

}

// Interrupt configuration and handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi) {

}
void GPIO_IRQHandling(uint8_t PinNumber) {

}

//-------------------------------------------
