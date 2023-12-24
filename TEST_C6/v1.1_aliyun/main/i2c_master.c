#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include"i2c_master.h"

 
/**
 * @brief i2c master initialization
 */
esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM; // Define the I2C master port number
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER, // Set the I2C mode as master
        .sda_io_num = I2C_MASTER_SDA_IO, // Set the SDA pin number
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // Enable the pull-up resistor for SDA pin
        .scl_io_num = I2C_MASTER_SCL_IO, // Set the SCL pin number
        .scl_pullup_en = GPIO_PULLUP_ENABLE, // Enable the pull-up resistor for SCL pin
        .master.clk_speed = I2C_MASTER_FREQ_HZ, // Set the clock speed for I2C communication
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf); // Configure I2C parameters
    if (err != ESP_OK) { // Check if configuration was successful
        return err;
    }
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0); // Install I2C driver
}

