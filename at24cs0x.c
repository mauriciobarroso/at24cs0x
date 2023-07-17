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
#include "esp_err.h"
#include "esp_log.h"

/* Private macros ------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const char *TAG = "at24cs0x";

/* Private function prototypes -----------------------------------------------*/

/* Exported functions definitions --------------------------------------------*/
/**
 * @brief Function to initialize a AT24CS0x instance
 */
esp_err_t at24cs0x_init(at24cs0x_t *const me, i2c_bus_t *i2c_bus,
		uint8_t dev_addr, i2c_bus_read_t read, i2c_bus_write_t write) {
	/* Print initializing message */
	ESP_LOGI(TAG, "Initializing AT24CS0X instance...");

	/* Variable to return */
	esp_err_t ret = ESP_OK;

	/* Add device to bus */
	ret = i2c_bus_add_dev(i2c_bus, dev_addr, "at24cs0x", NULL, NULL);

	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to add device");
		return ret;
	}

	/**/
	me->i2c_dev = &i2c_bus->devs.dev[i2c_bus->devs.num - 1];

	/* Print successful initialization message */
	ESP_LOGI(TAG, "AT24CS0X instance initialized successfully");

	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to write a byte
 */
esp_err_t at24cs0x_write_byte(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data) {
	/* Variable to return */
	esp_err_t ret = ESP_OK;

	int8_t rslt = me->i2c_dev->write(data_addr, data, 1, me->i2c_dev);

	if (rslt != I2C_BUS_OK) {
		ESP_LOGE(TAG, "Failed to write a byte in 0x%02X", data_addr);
		return ESP_FAIL;
	} else {
			printf("Error %d\n", rslt);
	}

	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to write a page of 8 bytes
 */
esp_err_t at24cs0x_write_page(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data) {
	/* Variable to return */
	esp_err_t ret = ESP_OK;

	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to read the current address
 */
esp_err_t at24cs0x_read_current(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data) {
	/* Variable to return */
	esp_err_t ret = ESP_OK;


	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to read the current address
 */
esp_err_t at24cs0x_read_random(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data) {
	/* Variable to return */
	esp_err_t ret = ESP_OK;

	/**/
	int8_t rslt = me->i2c_dev->read(data_addr, data, 1, me->i2c_dev);

	if (rslt != I2C_BUS_OK) {
		ESP_LOGE(TAG, "Failed to read a byte in 0x%02X", data_addr);
		return ESP_FAIL;
	}

	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to read the current address
 */
esp_err_t at24cs0x_read_sequential(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data) {
	/* Variable to return */
	esp_err_t ret = ESP_OK;

	/* Return ESP_OK */
	return ret;
}

/**
 * @brief Function to read the current address
 */
esp_err_t at24cs0x_read_serial_number(at24cs0x_t *const me) {
	/* Variable to return */
	esp_err_t ret = ESP_OK;

	/**/
	me->i2c_dev->addr = AT24CS0X_SN_I2C_ADDRESS; /* todo: write function to change the device address */
	int8_t rslt = me->i2c_dev->read(0x80, me->serial_number, AT24CS0X_SN_SIZE, me->i2c_dev);
	me->i2c_dev->addr = AT24CS0X_I2C_ADDRESS;

	if (rslt != I2C_BUS_OK) {
		ESP_LOGE(TAG, "Failed to read serial number");
		return ESP_FAIL;
	}

	/* Return ESP_OK */
	return ret;
}

/* Private function definitions ----------------------------------------------*/

/***************************** END OF FILE ************************************/
