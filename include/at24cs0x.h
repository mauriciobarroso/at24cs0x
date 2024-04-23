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
#include <stdint.h>
#include <stdbool.h>

#include "driver/i2c_master.h"

/* Exported Macros -----------------------------------------------------------*/
#define AT24CS0X_SN_SIZE				16		/*!< Serial number size in bytes */

#define AT24CS0X_I2C_ADDRESS		0x50	/*!< Slave address for AT24CS0X */
#define AT24CS0X_I2C_ADDRESS_SN	0x58	/*!< Slave address for AT24CS0X */

#define AT24CS0X_PAGE_SIZE			8			/*!< EEPROM page size in bytes */
#define AT24CS01_MAX_SIZE				128		/*!< EEPROM page size in bytes */
#define AT24CS02_MAX_SIZE				256		/*!< EEPROM page size in bytes */
#define AT24CS01_PAGES_NUM			AT24CS01_MAX_SIZE / \
																AT24CS0X_PAGE_SIZE	/*!< EEPROM page size in bytes */
#define AT24CS02_PAGES_NUM			AT24CS02_MAX_SIZE / \
																AT24CS0X_PAGE_SIZE	/*!< EEPROM page size in bytes */

#define AT24CS0X_WRITE_TIME			5	/*!< EEPROM write time in ms */

/* Exported typedef ----------------------------------------------------------*/
/*
 * @brief AT24CS0x device structure
 */
typedef enum {
	AT24CS01_MODEL,
	AT24CS02_MODEL
} at24cs0x_model_e;

/*
 * @brief AT24CS0x device structure
 */
typedef struct {
	i2c_master_dev_handle_t i2c_dev;	/*!< I2C device handle */
	i2c_master_dev_handle_t i2c_dev_sn;	/*!< I2C device handle */
	at24cs0x_model_e model;
	uint8_t word_addr_curr; /*!< I2C device handle */
} at24cs0x_t;

/* Exported variables --------------------------------------------------------*/
/**
 * @brief Function to initialize a AT24CS0x instance
 *
 * @param me             : Pointer to a structure instance of at24cs0x_t
 * @param i2c_bus_handle : Handle to the I2C bus to add this device
 * @param model          : AT24CS0x model, can be AT24CS01 or AT24CS02
 * @param addr           : I2C device address
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_init(at24cs0x_t *const me, i2c_master_bus_handle_t i2c_bus_handle,
		uint8_t dev_addr, at24cs0x_model_e model);

/**
 * @brief Function to write a specific quantity of words to EEPROM
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 * @param data_len  : Data length in words (bytes)
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_write(at24cs0x_t *const me, uint8_t data_addr,
		uint8_t *data, uint32_t data_len);

/**
 * @brief Function to read a specific quantity of words from EEPROM
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 * @param data_len  : Data length in words (bytes)
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_read(at24cs0x_t *const me, uint8_t data_addr,
		uint8_t *data, uint32_t data_len);

/**
 * @brief Function to read the current address
 *
 * @param me        : Pointer to a structure instance of at24cs0x_t
 * @param data_addr : Address to write data
 * @param data      : Data to write
 *
 * @return ESP_OK on success
 */
esp_err_t at24cs0x_read_serial_number(at24cs0x_t *const me, uint8_t *serial_number);

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* AT24CS0X_H_ */

/***************************** END OF FILE ************************************/
