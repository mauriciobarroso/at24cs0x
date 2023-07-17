/**
  ******************************************************************************
  * @file           : at24cs0x.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AT24CS0X_H_
#define AT24CS0X_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "driver/i2c.h"
#include "i2c_bus.h"

/* Exported Macros -----------------------------------------------------------*/
#define AT24CS0X_SN_SIZE				(16) /*!< Serial number size */

#define AT24CS0X_I2C_ADDRESS		0x50 /*!< Slave address for AT24CS0X */
#define AT24CS0X_SN_I2C_ADDRESS	0x58 /*!< Slave address for AT24CS0X */

/* Exported typedef ----------------------------------------------------------*/

/*
 * @brief AT24CS0x device structure
 */
typedef struct {
	i2c_bus_dev_t *i2c_dev;
	uint8_t serial_number[AT24CS0X_SN_SIZE];
} at24cs0x_t;

/* Exported variables --------------------------------------------------------*/
/**
 * @brief Function to initialize a AT24CS0x instance
 *
 * @param me      : Pointer to a structure instance of at24cs0x_t
 * @param i2c_bus : Pointer to a structure with the data to initialize the
 * 								    	   sensor as a I2C device
 * @param addr    : I2C device address
 * @param read    : I2C read function pointer
 * @param write   : I2C write function pointer
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_init(at24cs0x_t *const me, i2c_bus_t *i2c_bus,
		uint8_t dev_addr, i2c_bus_read_t read, i2c_bus_write_t write);

/**
 * @brief Function to write a byte
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_write_byte(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data);

/**
 * @brief Function to write a page of 8 bytes
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_write_page(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data);

/**
 * @brief Function to read the current address
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_read_current(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data);

/**
 * @brief Function to read the current address
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_read_random(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data);

/**
 * @brief Function to read the current address
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_read_sequential(at24cs0x_t *const me, uint8_t data_addr, uint8_t *data);

/**
 * @brief Function to read the current address
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_read_serial_number(at24cs0x_t *const me);

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* AT24CS0X_H_ */

/***************************** END OF FILE ************************************/
