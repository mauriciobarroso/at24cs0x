# EEPROM memory AT24CS0x (AT24CS01 and AT24CS02) driver for ESP-IDF and STM32CubeIDE
Driver compatible with ESP-IDF and STM32CubeIDE to use AT24CS0x EEPROM memory with embedded serial ID series

## Features
- Initialization function allows use a custom delay function to ensure compatibility with bare-metal and RTOS applications
- Algorithm to prevent overwriting within the same page and ensure data length does not exceed the available memory
- Multiple instances.

## How to use
### ESP-IDF
To use this component follow the next steps:

1. Configure the component in the project configuration menu (`idf.py menuconfig`)

`
Component config->AT24CS0x Configuration
`

2. Include the I2C driver component headers
```c
#include "driver/i2c_master.h"
#include "at24cs0x.h"
```
3. Declare a handle of I2C bus and an instance of the compoent
```c
i2c_master_bus_handle_t i2c_bus_handle; 
at24cs0x_t eeprom;
```

4. Define a custom delay function in miliseconds according your application
```c
/* Custom delay function */
void delay_ms(uint32_t time)
{
    vTaskDelay(pdMS_TO_TICKS(time));
}
```

5. In the main function initialize the I2C bus and the component instance
```c
/* Configure and create a new I2C bus */
i2c_master_bus_config_t i2c_bus_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_NUM_0,
    .scl_io_num = GPIO_NUM_48,
    .sda_io_num = GPIO_NUM_47,
    .glitch_ignore_cnt = 7
};

ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle));

/* Initialize AT24CS0x instance for AT24CS02 */
ESP_ERROR_CHECK(at24cs0x_init(&eeprom, i2c_bus_handle, AT24CS0X_I2C_ADDRESS, AT24CS0X_MODEL_02, delay_ms));
```

7. Perform a sequential random write
```c
/* Declare an array to write in EEPROM */
uint8_t data[32] = 0xFF;

/* Write data array in address 0xF */
ESP_ERROR_CHECK(at24cs0x_write(&eeprom, 0xF, data, 32));
```

### STM32CubeIDE
TBD

## License
MIT License

Copyright (c) 2025 Mauricio Barroso Benavides

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
