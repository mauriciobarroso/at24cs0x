/**
  ******************************************************************************
  * @file           : at24cs0x.c
  * @author         : Mauricio Barroso Benavides
  * @date           : Jul 14, 2023
  * @brief          : todo: write brief
  ******************************************************************************
  * @attention
  *
  * MIT License
  *
  * Copyright (c) 2023 Mauricio Barroso Benavides
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to
  * deal in the Software without restriction, including without limitation the
  * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  * sell copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  * 
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  * IN THE SOFTWARE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at24cs0x.h"

#ifdef ESP32_TARGET
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#endif /* ESP32_TARGET */

/* Private macros ------------------------------------------------------------*/
#define NOP() asm volatile ("nop")

/* External variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const char *TAG = "at24cs0x";

/* Private function prototypes -----------------------------------------------*/
/**
 * @brief Function that implements the default I2C read transaction
 *
 * @param addr     : Register address to be read
 * @param data     : Pointer to the data to be read from addr
 * @param data_len : Length of the data transfer
 * @param intf     : Pointer to the interface descriptor
 *
 * @return 0 if successful, non-zero otherwise
 */
static int8_t at24cs0x_reg_read(uint8_t addr, uint8_t *data, uint32_t data_len,
		void *intf);

/**
 * @brief Function that implements the default I2C write transaction
 *
 * @param addr     : Register address to be written
 * @param data     : Pointer to the data to be written to addr
 * @param data_len : Length of the data transfer
 * @param intf     : Pointer to the interface descriptor
 *
 * @return 0 if successful, non-zero otherwise
 */
static int8_t at24cs0x_reg_write(uint8_t addr, const uint8_t *data,
		uint32_t data_len, void *intf);

/**
 * @brief Function that implements a mili seconds delay
 *
 * @param time_ms: Time in us to delay
 */
static void delay_ms(uint32_t time_ms);

/* Exported functions definitions --------------------------------------------*/
/**
 * @brief Function to initialize a AT24CS0x instance
 */
int at24cs0x_init(at24cs0x_t *const me, void *i2c_handle, uint8_t dev_addr,
		at24cs0x_model_t model)
{
	/* Variable to return error code */
	int ret = 0;

#ifdef ESP32_TARGET
	/* Add device to I2C bus */
	i2c_device_config_t i2c_dev_conf = {
			.scl_speed_hz = 400000,
			.device_address = dev_addr
	};

	if (i2c_master_bus_add_device((i2c_master_bus_handle_t)i2c_handle,
			&i2c_dev_conf, &me->i2c_dev.handle) != 0) {
		ESP_LOGE(TAG, "Failed to add device to I2C bus");
		return ret;
	}

	i2c_dev_conf.device_address = AT24CS0X_I2C_ADDRESS_SN;

	if (i2c_master_bus_add_device((i2c_master_bus_handle_t)i2c_handle,
			&i2c_dev_conf, &me->i2c_dev_sn) != 0) {
		ESP_LOGE(TAG, "Failed to add device to I2C bus");
		return ret;
	}
#else
	me->i2c_dev.handle = (I2C_HandleTypeDef *)i2c_handle;
	me->i2c_dev.addr = AT24CS0X_I2C_ADDRESS;

	me->i2c_dev_sn.handle = (I2C_HandleTypeDef *)i2c_handle;
	me->i2c_dev_sn.addr = AT24CS0X_I2C_ADDRESS_SN;
#endif /* ESP32_TARGET */

	/* Return 0 */
	return ret;
}

/**
 * @brief Function to write a specific quantity of words to EEPROM
 */
int at24cs0x_write(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data,
		uint32_t data_len)
{
	/* Variable to return error code */
	int ret = 0;

	/* Check if the data to write is greater than the available memory */
	if (data_len > (0xFF - data_addr + 1)) {
//		ESP_LOGE(TAG, "The data to be written is greater than the available memory");
		return -1;
	}

	/* Update the current write addres to prevent overwritten */
	uint8_t addr_curr = data_addr;
	uint8_t page_curr = (int)(addr_curr / 8);
	uint32_t data_len_rem = data_len;
	uint8_t data_index = 0;
	uint8_t page_len_rem = ((page_curr + 1) * 8) - addr_curr;

	do {
		/* If the remaining data to be written is greater than the data avilable in
		 * the current page */
		if (data_len_rem > page_len_rem) {
			if (at24cs0x_reg_write(addr_curr, data + data_index, page_len_rem,
					&me->i2c_dev) != 0) {
				return -1;
			}

			data_index += page_len_rem;
			addr_curr = ++page_curr * 8;
			data_len_rem -= page_len_rem;
			page_len_rem = ((page_curr + 1) * 8) - addr_curr;
		}
		else {
			if (at24cs0x_reg_write(addr_curr, data + data_index, data_len_rem,
					&me->i2c_dev) != 0) {
				return -1;
			}

			data_len_rem = 0;
		}

		/* Time to write the data */
		delay_ms(AT24CS0X_WRITE_TIME_MS);
	} while (data_len_rem > 0);

	/* Return 0 */
	return ret;
}

/**
 * @brief Function to read a specific quantity of words from EEPROM
 */
int at24cs0x_read(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data,
		uint32_t data_len)
{
	/* Variable to return error code */
	int ret = 0;

	/* Check if the data to write is greater than the available memory */
	if (data_len > (0xFF - data_addr + 1)) {
		ESP_LOGE(TAG, "The data to be read is greater than the available memory");
		return -1;
	}

	/* Read the requested data */
	if (at24cs0x_reg_read(data_addr, data, data_len, &me->i2c_dev) < 0) {
		return -1;
	}

	/* Return 0 */
	return ret;
}

/**
 * @brief Function to read the current address
 */
int at24cs0x_read_serial_number(at24cs0x_t *const me, uint8_t *serial_number)
{
	/* Variable to return error code */
	int ret = 0;

	/* Read serial number */
	if (at24cs0x_reg_read(0x80, serial_number, AT24CS0X_SN_SIZE, &me->i2c_dev_sn)
			< 0) {
		return -1;
	}

	/* Return 0 */
	return ret;
}

/* Private function definitions ----------------------------------------------*/
/**
 * @brief Function that implements the default I2C read transaction
 */
static int8_t at24cs0x_reg_read(uint8_t addr, uint8_t *data, uint32_t data_len,
		void *intf)
{
	at24cs0x_i2c_t *i2c_dev = (at24cs0x_i2c_t *)intf;

#ifdef ESP32_TARGET
	if (i2c_master_transmit_receive(i2c_dev->handle, &addr, 1, data, data_len,
			-1) != 0) {
		return -1;
	}
#else
	if (HAL_I2C_Master_Receive(i2c_dev->handle, (i2c_dev->addr << 1) | 0x01,
			data, data_len, 100) > 0) {
		return -1;
	}
#endif

	return 0;
}


/**
 * @brief Function that implements the default I2C write transaction
 */
static int8_t at24cs0x_reg_write(uint8_t addr, const uint8_t *data,
		uint32_t data_len, void *intf)
{
	uint8_t buffer[32] = {0};
	uint8_t addr_len = sizeof(addr);

	/* Copy the register address to buffer */
	for (uint8_t i = 0; i < addr_len; i++) {
		buffer[i] = (addr & (0xFF << ((addr_len - 1 - i) * 8)))
				>> ((addr_len - 1 - i) * 8);
	}

	/* Copy the data to buffer */
	for (uint8_t i = 0; i < data_len; i++) {
		buffer[i + addr_len] = data[i];
	}

	/* Transmit buffer */
	at24cs0x_i2c_t *i2c_dev = (at24cs0x_i2c_t *)intf;

#ifdef ESP32_TARGET
	if (i2c_master_transmit(i2c_dev->handle, buffer, addr_len + data_len, -1)
			!= 0) {
		return -1;
	}
#else
	if (HAL_I2C_Master_Transmit(i2c_dev->handle, (i2c_dev->addr << 1),
			buffer, addr_len + data_len, 100) > 0) {
		return -1;
	}
#endif

	return 0;
}

/**
 * @brief Function that implements a mili seconds delay
 */
static void delay_ms(uint32_t time_ms)
{
#ifdef ESP32_TARGET
	uint64_t m = (uint64_t)esp_timer_get_time();

	uint32_t period_us = time_ms * 1000;
	if (period_us) {
		uint64_t e = (m + period_us);

		if (m > e) { /* overflow */
			while ((uint64_t)esp_timer_get_time() > e) {
				NOP();
			}
		}

		while ((uint64_t)esp_timer_get_time() < e) {
			NOP();
		}
	}
#else
  HAL_Delay(time_ms);
#endif /* ESP32_TARGET */
}

/***************************** END OF FILE ************************************/
