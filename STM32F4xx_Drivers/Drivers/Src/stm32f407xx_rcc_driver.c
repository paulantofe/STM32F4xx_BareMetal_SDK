/**
 * @file 		stm32f407xx_rcc_driver.c
 * @brief 		Source file for RCC peripheral support
 *
 * @details     This source file contains the implementation of the APIs for the RCC peripheral on the MCU.
 */

#include "stm32f407xx_rcc_driver.h"

/* -------------------------- PRIVATE HELPER ARRAYS -------------------------- */

static uint16_t AHB_Prescaler[] = { 2, 4, 8, 16, 64, 128, 256, 512 };
static uint8_t  APB_Prescaler[] = { 2, 4, 8, 16 };

/* ----------------------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Get the frequency of the APB1 bus
 * @retval Frequency of APB1 bus
 */
uint32_t RCC_GetPCLK1Value(void) {
	uint32_t pclk1, system_clk;
	uint8_t clk_source, temp, apb_pre;
	uint16_t ahb_pre;

	clk_source = (RCC->CFGR >> 2) & 0x3;

	if (clk_source == 0) {
		// HSI selected
		system_clk = HSI_VALUE;
	}
	else if (clk_source == 1) {
		// HSE selected
		system_clk = HSE_VALUE;
	}
	else if (clk_source == 2) {
		// PLL selected
		// Not supported by the custom driver layer
	}

	// AHB Clock Prescaler
	temp = (RCC->CFGR >> 4) & 0xF;
	if (temp < 8) {
		ahb_pre = 1;
	}
	else {
		ahb_pre = AHB_Prescaler[temp - 8];
	}

	// APB Clock Prescaler
	temp = (RCC->CFGR >> 10) & 0x7;
	if (temp < 4) {
		apb_pre = 1;
	}
	else {
		apb_pre = APB_Prescaler[temp - 4];
	}

	pclk1 = (system_clk / ahb_pre) / apb_pre;

	return pclk1;
}

/**
 * @brief  Get the frequency of the APB2 bus
 * @retval Frequency of APB2 bus
 */
uint32_t RCC_GetPCLK2Value(void) {
	uint32_t pclk2, system_clk;
	uint8_t clk_source, temp, apb_pre;
	uint16_t ahb_pre;

	clk_source = (RCC->CFGR >> 2) & 0x3;

	if (clk_source == 0) {
		// HSI selected
		system_clk = HSI_VALUE;
	}
	else if (clk_source == 1) {
		// HSE selected
		system_clk = HSE_VALUE;
	}
	else if (clk_source == 2) {
	    // PLL selected
	    // Not supported by the custom driver layer
	}

	// AHB Clock Prescaler
	temp = (RCC->CFGR >> 4) & 0xF;
	if (temp < 8) {
		ahb_pre = 1;
	}
	else {
		ahb_pre = AHB_Prescaler[temp - 8];
	}

	// APB Clock Prescaler
	temp = (RCC->CFGR >> 13) & 0x7;
	if (temp < 4) {
		apb_pre = 1;
	}
	else {
		apb_pre = APB_Prescaler[temp - 4];
	}

	pclk2 = (system_clk / ahb_pre) / apb_pre;

	return pclk2;
}

/* ----------------------------------------------------------------------------------- */
