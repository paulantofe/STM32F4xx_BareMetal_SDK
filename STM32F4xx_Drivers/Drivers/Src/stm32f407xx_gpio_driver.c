/**
 * @file 		stm32f407xx_gpio_driver.c
 * @brief 		Source file for GPIO peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the GPIO peripherals on the MCU. It includes the functional
 *              code for peripheral clock control, GPIO initialization and de-initialization, read/write operations on pins and ports,
 *              as well as interrupt configuration and handling
 */

#include "stm32f407xx_gpio_driver.h"

/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Enable/disable peripheral clock for the given GPIO port
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @param  EnorDi	ENABLE or DISABLE macros
 * @retval None
 */
void GPIO_PCLKControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi) {
	if (pGPIOx == NULL) { return; }

	uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOx);

	if (EnorDi == ENABLE) {
		RCC->AHB1ENR |= (1 << portCode);
	}
	else {
		RCC->AHB1ENR &= ~(1 << portCode);
	}
}

/**
 * @brief  Initialize a GPIO pin with the given settings
 * @param  pGPIOHandle	Handle structure with the desired settings
 * @retval None
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {
	if (pGPIOHandle == NULL || pGPIOHandle->pGPIOx == NULL) { return; }

	uint32_t temp = 0;
	uint32_t pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

	// Mode Configuration
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		// Non-Interrupt Mode
		temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode  << GPIO_MODER_PIN_POS(pinNumber);
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << GPIO_MODER_PIN_POS(pinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;
	}
	else {
		// Interrupt Mode (forces pin into input mode implicitly)
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << GPIO_MODER_PIN_POS(pinNumber));

		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT) {
			EXTI->RTSR &= ~(1 << pinNumber);
			EXTI->FTSR |= (1 << pinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
			EXTI->FTSR &= ~(1 << pinNumber);
			EXTI->RTSR |= (1 << pinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT) {
			EXTI->FTSR |= (1 << pinNumber);
			EXTI->RTSR |= (1 << pinNumber);
		}

		// SYSCFG Configuration for EXTICR
		uint32_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint32_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] &= ~(0xF << (temp2 * 4));
		SYSCFG->EXTICR[temp1] |= portCode << (temp2 * 4);

		// Enable EXTI interrupt delivery
		EXTI->IMR |= (1 << pinNumber);
 	}

	// OType and Speed Configuration (only valid for Output/AF modes)
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_OUTPUT ||
	    pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
		// OType Configuration
		temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOType << GPIO_OTYPER_PIN_POS(pinNumber);
		pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << GPIO_OTYPER_PIN_POS(pinNumber));
		pGPIOHandle->pGPIOx->OTYPER |= temp;

		// Speed Configuration
		temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << GPIO_OSPEEDR_PIN_POS(pinNumber);
		pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << GPIO_OSPEEDR_PIN_POS(pinNumber));
		pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	}

	// PuPd Configuration
	temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << GPIO_PUPDR_PIN_POS(pinNumber);
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << GPIO_PUPDR_PIN_POS(pinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	// AF Configuration
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
		if (pinNumber <= 7) {
			temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinAFMode << GPIO_AFRL_PIN_POS(pinNumber);
			pGPIOHandle->pGPIOx->AFR[0] &= ~(0xF << GPIO_AFRL_PIN_POS(pinNumber));
			pGPIOHandle->pGPIOx->AFR[0] |= temp;
		}
		else {
			temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinAFMode << GPIO_AFRH_PIN_POS(pinNumber);
			pGPIOHandle->pGPIOx->AFR[1] &= ~(0xF << GPIO_AFRH_PIN_POS(pinNumber));
			pGPIOHandle->pGPIOx->AFR[1] |= temp;
		}
	}
}

/**
 * @brief  De-initialize (reset) and entire GPIO peripheral
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @retval None
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {
	if (pGPIOx == NULL) { return; }

	uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOx);

	RCC->AHB1RSTR |= (1 << portCode);
	RCC->AHB1RSTR &= ~(1 << portCode);
}

/**
 * @brief  Read from an input pin
 * @param  pGPIOx		Base address of the GPIO peripheral
 * @param  PinNumber	Number of the read pin
 * @retval Value of the input pin
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
	return (pGPIOx->IDR & (1 << PinNumber)) != 0;
}

/**
 * @brief  Read from an input port
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @retval Values of the pins in the input port
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {
	return (uint16_t) pGPIOx->IDR;
}

/**
 * @brief  Write value to an output pin
 * @param  pGPIOx		Base address of the GPIO peripheral
 * @param  PinNumber	Number of the output pin
 * @param  Value 		Value which is desired to be written to the pin
 * @retval None
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value) {
	if (Value == GPIO_PIN_SET)
		pGPIOx->BSRR = (1 << PinNumber);
	else
		pGPIOx->BSRR = (1 << (PinNumber + 16));
}

/**
 * @brief  Write values to an output port
 * @param  pGPIOx	Base address of the GPIO peripheral
 * @param  Value	Value which is desired to be written to the port
 * @retval None
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value) {
	pGPIOx->ODR = Value;
}

/**
 * @brief  Toggle an output pin
 * @param  pGPIOx		Base address of the GPIO peripheral
 * @param  PinNumber	Number of the output pin
 * @retval None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
	pGPIOx->ODR ^= (1 << PinNumber);
}

/**
 * @brief  Configure an interrupt on a GPIO pin
 * @param  IRQNumber	Number of the interrupt request
 * @param  EnorDi		ENABLE/DISABLE macros
 * @retval None
 */
void GPIO_IRQInterruptConfig(IRQn_Type IRQNumber, uint8_t EnorDi) {
	if (IRQNumber > 81) {
		return;
	}

	if (EnorDi == ENABLE) {
		NVIC_ISER_BASEADDR[IRQNumber / 32] |= (1 << (IRQNumber % 32));
	}
	else {
		NVIC_ICER_BASEADDR[IRQNumber / 32] |= (1 << (IRQNumber % 32));
	}
}

/**
 * @brief  Set interrupt priority for a GPIO pin
 * @param  IRQNumber	Number of the interrupt request
 * @param  IRQPriority	Priority of the interrupt
 * @retval None
 */
void GPIO_IRQPriorityConfig(IRQn_Type IRQNumber, uint8_t IRQPriority) {
	if (IRQNumber > 81 || IRQPriority > 15) {
		return;
	}

	uint8_t iprx = IRQNumber / 4;
	uint8_t iprxSection = IRQNumber % 4;
	uint8_t shiftAmount = 8 * iprxSection + (8 - NO_PR_BITS_IMPLEMENTED);
	NVIC_IPR_BASEADDR[iprx] |= (IRQPriority << shiftAmount);
}

/**
 * @brief  Clear the EXTI_PR bit corresponding to the PinNumber
 * @param  PinNumber	Number of the pin that triggered the interrupt
 * @retval None
 */
void GPIO_IRQHandling(uint8_t PinNumber) {
	if (EXTI->PR & (1 << PinNumber)) {
		EXTI->PR = (1 << PinNumber);
	}
}

/* ----------------------------------------------------------------------------------- */
