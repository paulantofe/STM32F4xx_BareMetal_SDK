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

static uint8_t binary_to_bcd(uint8_t bin_value) {
	return (uint8_t) ((bin_value / 10 << 4) | (bin_value % 10));
}

static uint8_t bcd_to_binary(uint8_t bcd_value) {
	return (uint8_t) (((bcd_value >> 4) * 10) + (bcd_value & 0x0F));
}

/* ----------------------------------------------------------------------------------- */


/* -------------------------- SUPPORTED APIs IMPLEMENTATION -------------------------- */

/**
 * @brief  Initialize DS1307 chip
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
 * @param  pRTC_Time   pointer to RTC_Date_Time_t data structure
 *                     with desired configuration
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

/**
 * @brief  Set current date
 * @param  pRTC_Date   pointer to RTC_Date_Time_t data structure
 *                     with desired configuration
 */
void DS1307_SetDate(RTC_Date_Time_t *pRTC_Date) {
	ds1307_write(binary_to_bcd(pRTC_Date->date), DS1307_ADDR_DATE);

	ds1307_write(binary_to_bcd(pRTC_Date->day), DS1307_ADDR_DAY);

	ds1307_write(binary_to_bcd(pRTC_Date->month), DS1307_ADDR_MONTH);

	ds1307_write(binary_to_bcd(pRTC_Date->year), DS1307_ADDR_YEAR);
}

/**
 * @brief  Get current time
 * @param  pRTC_Time   pointer to RTC_Date_Time_t data structure
 *                     to get info into
 */
void DS1307_GetTime(RTC_Date_Time_t *pRTC_Time) {
	uint8_t time_unit;

	// Read seconds
	time_unit = ds1307_read(DS1307_ADDR_SEC);
	// Clear CH bit as it`s not relevant for seconds
	time_unit &= ~(1 << DS1307_SEC_CH_POS);
	pRTC_Time->seconds = bcd_to_binary(time_unit);

	// Read minutes
	time_unit = ds1307_read(DS1307_ADDR_MIN);
	pRTC_Time->minutes = bcd_to_binary(time_unit);

	// Read hours
	time_unit = ds1307_read(DS1307_ADDR_HRS);
	if (time_unit & (1 << DS1307_HRS_FORMAT_POS)) {
		// 12 hr format
		if (time_unit & (1 << DS1307_HRS_AM_PM_POS)) {
			// pm
			pRTC_Time->time_format = DS1307_TIME_FORMAT_12H_PM;
		}
		else {
			// am
			pRTC_Time->time_format = DS1307_TIME_FORMAT_12H_AM;
		}
		// Clear time format bit and am/pm bit
		time_unit &= ~(0x3 << DS1307_HRS_AM_PM_POS);
	}
	else {
		// 24 hr format
		pRTC_Time->time_format = DS1307_TIME_FORMAT_24H;
	}
	pRTC_Time->hours = bcd_to_binary(time_unit);
}

/**
 * @brief  Get current date
 * @param  pRTC_Date   pointer to RTC_Date_Time_t data structure
 *                     to get info into
 */
void DS1307_GetDate(RTC_Date_Time_t *pRTC_Date) {
	pRTC_Date->day = bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));

	pRTC_Date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));

	pRTC_Date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));

	pRTC_Date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));
}

/**
 * @brief  Enable/Disable Square Wave output
 * @param  EnorDi   ENABLE/DISABLE macros
 */
void DS1307_ManageSquareWave(uint8_t EnorDi) {
	uint8_t ctrl_reg;

	ctrl_reg = ds1307_read(DS1307_ADDR_CTRL);

	if (EnorDi == ENABLE) {
		ctrl_reg |= (1 << DS1307_CTRL_SQWE_POS);
	}
	else {
		ctrl_reg &= ~(1 << DS1307_CTRL_SQWE_POS);
	}

	ds1307_write(ctrl_reg, DS1307_ADDR_CTRL);
}

/**
 * @brief  Set frequency of the square wave output
 * @param  Freq   Frequency macro @ref DS1307_SQ_FREQ
 */
void DS1307_SquareWaveFrequency(uint8_t Freq) {
	uint8_t ctrl_reg = ds1307_read(DS1307_ADDR_CTRL);

	ctrl_reg &= ~(0x3 << DS1307_CTRL_RS0_POS);
	ctrl_reg |= (Freq << DS1307_CTRL_RS0_POS);

	ds1307_write(ctrl_reg, DS1307_ADDR_CTRL);
}

/* ----------------------------------------------------------------------------------- */
