/**
 * @file 		ds1307.c
 * @brief 		Source file for the DS1307 Real-Time Clock BSP driver.
 *
 * @details     This source file contains the implementation of the APIs for communicating with the DS1307
 *              RTC module over I2C. It includes functional code for device initialization, reading and
 *              writing time and date registers, as well as helper functions for BCD (Binary-Coded Decimal)
 *              conversions.
 */

#include "ds1307.h"

/* -------------------------- PRIVATE HELPER FUNCTIONS -------------------------- */

static I2C_Handle_t com = { 0 };

static void ds1307_i2c_pins_init() {
	GPIO_PClkControl(DS1307_I2C_GPIO_PORT, ENABLE);

	GPIO_Handle_t i2c_pin = { 0 };
	i2c_pin.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_pin.GPIO_PinConfig.GPIO_PinAFMode = GPIO_AF4;
	i2c_pin.GPIO_PinConfig.GPIO_PinOType = GPIO_OP_TYPE_OD;
	i2c_pin.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SDA Initialization
	i2c_pin.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	GPIO_Init(&i2c_pin);

	// SCL Initialization
	i2c_pin.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	GPIO_Init(&i2c_pin);
}

static void ds1307_i2c_peripheral_init() {
	I2C_PClkControl(DS1307_I2C, ENABLE);

	com.pI2Cx = DS1307_I2C;
	com.I2C_Config.I2C_AckControl = I2C_ACK_EN;
	com.I2C_Config.I2C_SclSpeed = DS1307_I2C_SPEED;
	com.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;

	I2C_Init(&com);
	I2C_PeripheralControl(DS1307_I2C, ENABLE);
	I2C_ManageAcking(DS1307_I2C, ENABLE);
}

static void ds1307_write(uint8_t value, uint8_t reg_addr) {
	uint8_t tx[2] = { reg_addr, value };

	I2C_MasterSendData(&com, tx, 2, DS1307_I2C_ADDR, I2C_SR_DI);
}

static uint8_t ds1307_read(uint8_t reg_addr) {
	uint8_t data;

	I2C_MasterSendData(&com, &reg_addr, 1, DS1307_I2C_ADDR, I2C_SR_EN);
	I2C_MasterReceiveData(&com, &data, 1, DS1307_I2C_ADDR, I2C_SR_DI);

	return data;
}

/* ----------------------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Initialize DS1307 chip
 * @retval None
 */
void DS1307_Init(void) {
	ds1307_i2c_pins_init();

	ds1307_i2c_peripheral_init();

	// Enable crystal oscillator in DS1307 chip
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);
	clock_state &= ~(1 << DS1307_SEC_CH_POS);
	ds1307_write(clock_state, DS1307_ADDR_SEC);
}

/**
 * @brief  Set current time
 * @retval None
 */
void DS1307_SetTime(RTC_Date_Time_t *pRTC_Time) {
	uint8_t time_unit;

	// Set seconds
	time_unit = binary_to_bcd(pRTC_Time->seconds);
	// Make sure RTC clock is not halted
	time_unit &= ~(1 << DS1307_SEC_CH_POS);
	ds1307_write(time_unit, DS1307_ADDR_SEC);

	// Set minutes
	time_unit = binary_to_bcd(pRTC_Time->minutes);
	ds1307_write(time_unit, DS1307_ADDR_MIN);

	// Set hours
	time_unit = binary_to_bcd(pRTC_Time->hours);
	if (pRTC_Time->time_format == DS1307_TIME_FORMAT_24H) {
		time_unit &= ~(1 << DS1307_HRS_FORMAT_POS);
	}
	else {
		time_unit |= (1 << DS1307_HRS_FORMAT_POS);
		if (pRTC_Time->time_format == DS1307_TIME_FORMAT_12H_PM) {
			time_unit |= (1 << DS1307_HRS_AM_PM_POS);
		}
		else {
			time_unit &= ~(1 << DS1307_HRS_AM_PM_POS);
		}
	}
	ds1307_write(time_unit, DS1307_ADDR_HRS);
}


/* ----------------------------------------------------------------------------------- */
