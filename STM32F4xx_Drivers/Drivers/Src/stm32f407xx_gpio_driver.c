#include "stm32f407xx_gpio_driver.h"

/* ------------------- SUPPORTED APIs IMPLEMENTATION ------------------- */

/**
 * @brief  Enable/disable peripheral clock for the given GPIO port
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @param  EnorDi	ENABLE or DISABLE macros
 * @retval None
 */
void GPIO_PCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {

}

/**
 * @brief  Initialize a GPIO pin with the given settings
 * @param  pGPIOHandle	Handle structure with the desired settings
 * @retval None
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {

}

/**
 * @brief  Reset a GPIO pin
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @retval None
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {

}

/**
 * @brief  Read from an input pin
 * @param  pGPIOx		Base address of the GPIO peripheral
 * @param  PinNumber	Number of the read pin
 * @retval Value of the input pin
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {

}

/**
 * @brief  Read from an input port
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @retval Values of the pins in the input port
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {

}

/**
 * @brief  Write value to an output pin
 * @param  pGPIOx		Base address of the GPIO peripheral
 * @param  PinNumber	Number of the output pin
 * @param  Value 		Value which is desired to be written to the pin
 * @retval None
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value) {

}

/**
 * @brief  Write values to an output port
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @param  Value	Value which is desired to be written to the port
 * @retval None
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value) {

}

/**
 * @brief  Toggle an output pin
 * @param  pGPIOx		Base address of the GPIO peripheral
 * @param  PinNumber	Number of the output pin
 * @retval None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {

}

/**
 * @brief  Configure an interrupt on a GPIO pin
 * @param  IRQNumber	Number of the interrupt request
 * @param  IRQPriority	Priority of the interrupt
 * @param  EnorDi		ENABLE/DISABLE macros
 * @retval None
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi) {

}

/**
 * @brief  Handle the interrupt for a specific GPIO pin
 * @param  PinNumber	Number of the pin that triggered the interrupt
 * @retval None
 */
void GPIO_IRQHandling(uint8_t PinNumber) {

}

//-------------------------------------------
