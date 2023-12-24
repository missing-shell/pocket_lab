#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include <math.h>
#include "i2c_master.h"
#include "i2c_senor.h"

static const char *BH1750_TAG = "BH1750";
static const char *SHT35_TAG = "SHT35_DIS_B";
static const char *MCP4725_TAG = "MCP4725";
static const char *INA3221_TAG = "INA3221";

float bh1760_lux;
double sht35_val[2];
int dac_val;
double bus_val[3], shunt_val[3];


/*************************************************BH1750**********************************************************/
/*test code to operate on BH1750 sensor*/
static esp_err_t i2c_master_sensor_BH1750(i2c_port_t i2c_num, uint8_t *data_h, uint8_t *data_l)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();                                  // Create an I2C command handle
    i2c_master_start(cmd);                                                         // Start signal
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN); // Write slave address + write bit and enable ACK checking
    i2c_master_write_byte(cmd, BH1750_CMD_START, ACK_CHECK_EN);                    // Write command to start measurement
    i2c_master_stop(cmd);                                                          // Stop signal
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);           // Execute the I2C command
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    { // Check if operation was successful
        return ret;
    }
    vTaskDelay(30 / portTICK_PERIOD_MS); // Delay before reading data
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);                                                        // Start signal
    i2c_master_write_byte(cmd, BH1750_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN); // Write slave address + read bit and enable ACK checking
    i2c_master_read_byte(cmd, data_h, ACK_VAL);                                   // Read MSB data with ACK
    i2c_master_read_byte(cmd, data_l, NACK_VAL);                                  // Read LSB data without ACK
    i2c_master_stop(cmd);                                                         // Stop signal
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);          // Execute the I2C command
    i2c_cmd_link_delete(cmd);
    return ret;
}

/*BH1750*/
void i2c_BH1750_task(void *arg)
{
    int ret;
    //int task_idx = (int)arg;
    uint8_t sensor_data_h = 0, sensor_data_l = 0; // BH1750
    while (1)
    {
        ret = i2c_master_sensor_BH1750(I2C_MASTER_NUM, &sensor_data_h, &sensor_data_l); // Perform I2C sensor test
        if (ret == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(BH1750_TAG, "I2C Timeout"); // Print error message if I2C timeout occurs
        }
        else if (ret == ESP_OK)
        {
            bh1760_lux = (sensor_data_h << 8 | sensor_data_l) / 1.2;
        }
        else
        {
            ESP_LOGW(BH1750_TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret)); // Print warning message if no acknowledgement is received
        }
        //vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_PERIOD_MS); // Delay between different test items
        //---------------------------------------------------
    }
    vTaskDelete(NULL);
}
/*************************************************BH1750**********************************************************/

/*************************************************SHT35**********************************************************/
/*test code to operate on SHT35-DIS-B sensor*/
static esp_err_t i2c_master_sensor_SHT35(i2c_port_t i2c_num, uint8_t *data)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Create an I2C command handle
    // 1. Set measurement mode
    // _________________________________________________________________
    // | start | slave_addr + wr_bit + ack | write 2 bytes + ack  | stop |
    // --------|---------------------------|---------------------|------|
    i2c_master_start(cmd);                                                              // Start signal
    i2c_master_write_byte(cmd, SHT35_DIS_B_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN); // Write slave address + write bit and enable ACK checking
    i2c_master_write_byte(cmd, SHT35_DIS_B_CMD_START >> 8, ACK_CHECK_EN);               // write high byte of command
    i2c_master_write_byte(cmd, SHT35_DIS_B_CMD_START & 0xFF, ACK_CHECK_EN);             // write low byte of command
    i2c_master_stop(cmd);                                                               // Stop signal
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);                // Execute the I2C command
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    { // Check if operation was successful
        return ret;
    }
    // 2. Wait for measurement to complete (the exact time depends on the measurement mode)
    vTaskDelay(50 / portTICK_PERIOD_MS); // Delay before reading data
    cmd = i2c_cmd_link_create();
    // 3. Read measurement data
    // ______________________________________________________________________________________
    // | start | slave_addr + rd_bit + ack | read 2 bytes + ack (temperature) | read 2 bytes + ack (humidity) | stop |
    // --------|---------------------------|--------------------|--------------------|------|
    i2c_master_start(cmd);                                                             // Start signal
    i2c_master_write_byte(cmd, SHT35_DIS_B_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN); // Write slave address + read bit and enable ACK checking

    i2c_master_read_byte(cmd, &data[0], ACK_VAL);                        // Read first byte with ACK
    i2c_master_read_byte(cmd, &data[1], ACK_VAL);                        // Read second byte with ACK
    i2c_master_read_byte(cmd, &data[2], ACK_VAL);                        // Read CRC for temperature
    i2c_master_read_byte(cmd, &data[3], ACK_VAL);                        // Read third byte with ACK
    i2c_master_read_byte(cmd, &data[4], ACK_VAL);                        // Read fourth byte with ACK
    i2c_master_read_byte(cmd, &data[5], NACK_VAL);                       // Read CRC for humidity
    i2c_master_stop(cmd);                                                // Stop signal
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS); // Execute the I2C command
    i2c_cmd_link_delete(cmd);
    return ret;
}

/*SHT35_CRC*/
static uint8_t calculate_crc(uint8_t *data, uint8_t len)
{
    // CRC initial value
    uint8_t crc = 0xFF;

    // Calculate 8-Bit checksum with the given polynomial
    for (uint8_t byteCtr = 0; byteCtr < len; byteCtr++)
    {
        crc ^= (data[byteCtr]);
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                // If the leftmost (most significant) bit is '1', take it out (via XOR with a shifted mask)
                // and replace it with the remainder of the division (via XOR) of the polynomial
                crc = (crc << 1) ^ 0x131;
            }
            else
            {
                // If the leftmost bit is '0', just shift the register contents
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

static void i2c_SHT35_task(void *arg)
{
    int ret;
    //int task_idx = (int)arg;
    uint8_t buff[6];
    while (1)
    {
        ret = i2c_master_sensor_SHT35(I2C_MASTER_NUM, buff); // Perform I2C sensor test
        // xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(SHT35_TAG, "I2C Timeout"); // Print error message if I2C timeout occurs
        }
        else if (calculate_crc(&buff[0], 2) != buff[2] || calculate_crc(&buff[3], 2) != buff[5])
        {
            printf("CRC_ERROR,ret = 0x%x\r\n", ret);
        }
        else if (ret == ESP_OK)
        {
            uint16_t rawTemperature = (((uint16_t)buff[0] << 8) | buff[1]);          // Combine the two bytes to get the raw temperature value
            uint16_t rawHumidity = (((uint16_t)buff[3] << 8) | buff[4]);             // Combine the two bytes to get the raw humidity value
            double actualTemperature = -45 + 175 * ((double)rawTemperature / 65536); // Convert raw temperature to actual temperature
            double actualHumidity = 100 * ((double)rawHumidity / 65536);             // Convert raw humidity to actual humidity
            sht35_val[0] = actualTemperature;
            sht35_val[1] = actualHumidity;
        }
        else
        {
            ESP_LOGW(SHT35_TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret)); // Print warning message if no acknowledgement is received
        }
        // xSemaphoreGive(print_mux);
        //vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_PERIOD_MS); // Delay between different test items
        //---------------------------------------------------
    }
    // vSemaphoreDelete(print_mux);
    vTaskDelete(NULL);
}
/*************************************************SHT35**********************************************************/

/*************************************************MCP4725**********************************************************/
// Function to write a DAC value to the MCP4725
static esp_err_t i2c_master_mcp4725_write(i2c_port_t i2c_num, uint16_t dac_value)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();                                     // Create an I2C command handle
    i2c_master_start(cmd);                                                            // Start signal                                                           // Start signal
    i2c_master_write_byte(cmd, (MCP4725_SENSOR_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN); // Write slave address + write bit and enable ACK checking
    i2c_master_write_byte(cmd, MCP4725_CMD_WRITE, ACK_CHECK_EN);                      // Write the write command
    i2c_master_write_byte(cmd, dac_value >> 4, ACK_CHECK_EN);                         // Write the high byte of the DAC value
    i2c_master_write_byte(cmd, dac_value << 4, ACK_CHECK_EN);                         // Write the low byte of the DAC value
    i2c_master_stop(cmd);                                                             // Stop signal
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);              // Execute the I2C command
    i2c_cmd_link_delete(cmd);                                                         // Delete the I2C command handle
    return ret;
}
static void i2c_MCP4725_task(void *arg)
{
    int ret;
    // int task_idx = (int)arg;
    double angle = 0.0;
    while (1)
    {
        // Calculate the value of the sine wave
        double sin_val = (sin(angle) + 1.0) / 2.0;                // Shift range from -1~1 to 0~1
        uint16_t dac_value = (uint16_t)(sin_val * DAC_MAX + 0.5); // Scale to range 0~4095

        // Write the DAC value
        ret = i2c_master_mcp4725_write(I2C_MASTER_NUM, dac_value);
        if (ret != ESP_OK) // Check if the operation was successful
        {
            ESP_LOGE(MCP4725_TAG, "MCP4725 Write Failed"); // Print an error message if the operation failed
        }
        else
        {
            dac_val = dac_value;
        }
        // Increase the angle
        angle += 0.01;
        if (angle > 2 * PI)
        {
            angle -= 2 * PI;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
/*************************************************MCP4725**********************************************************/

/*************************************************INA3221**********************************************************/
static esp_err_t i2c_master_read_from_reg(i2c_cmd_handle_t cmd, uint8_t device_addr, uint8_t reg_addr, uint8_t *data_h, uint8_t *data_l)
{
    esp_err_t ret;
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    return ret;
}
static esp_err_t i2c_master_sensor_test(i2c_port_t i2c_num, double bus_voltage[3], double shunt_voltage[3])
{
    int ret;
    uint8_t data_h = 0, data_l = 0;
    uint16_t rawVoltage;
    i2c_cmd_handle_t cmd;

    // Read bus voltage and shunt voltage for each channel
    for (int i = 0; i < 3; i++)
    {
        // Read bus voltage
        cmd = i2c_cmd_link_create();
        ret = i2c_master_read_from_reg(cmd, INA3221_SENSOR_ADDR, REG_BUS_VOLTAGE_CHANNEL1 + i * 2, &data_h, &data_l);
        rawVoltage = (data_h << 8) | data_l;        // Combine high byte and low byte
        bus_voltage[i] = (rawVoltage >> 3) * 0.008; // Multiply by 8mV to get the actual voltage
        i2c_cmd_link_delete(cmd);                   // Always delete the cmd link after reading

        // Read shunt voltage
        cmd = i2c_cmd_link_create();
        ret = i2c_master_read_from_reg(cmd, INA3221_SENSOR_ADDR, REG_SHUNT_VOLT_CHANNEL1 + i * 2, &data_h, &data_l);
        rawVoltage = (data_h << 8) | data_l; // Combine high byte and low byte
        if (rawVoltage & 0x8000)
        {                             // If the most significant bit is set
            rawVoltage |= 0xFFFF0000; // Extend the sign bit
        }
        shunt_voltage[i] = (rawVoltage)*0.00004; // Convert raw shunt voltage to actual voltage
        i2c_cmd_link_delete(cmd);                // Always delete the cmd link after reading，
    }

    return ESP_OK;
}
static void i2c_INA3221_task(void *arg)
{
    //int task_idx = (int)arg;
    int ret;
    double bus_voltage[3], shunt_voltage[3];
    double shunt_resistance = 100.0;
    double current[3];
    double power[3];
    while (1)
    {
        ret = i2c_master_sensor_test(I2C_MASTER_NUM, &bus_voltage, &shunt_voltage); // Call the new function to read the INA3221's data
        // xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(INA3221_TAG, "I2C Timeout"); // Print error message if I2C timeout occurs
        }
        else if (ret == ESP_OK)
        {
            for (int i = 0; i < 3; i++)
            {
                current[i] = shunt_voltage[i] / shunt_resistance;
                power[i] = bus_voltage[i] * current[i];
                bus_val[i]=bus_voltage[i];
                shunt_val[i]=shunt_voltage[i];
            }
        }
        else
        {
            ESP_LOGW(INA3221_TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret)); // Print warning message if no acknowledgement is received
        }
        // xSemaphoreGive(print_mux);
        // vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_PERIOD_MS); // Delay between different test items
        //---------------------------------------------------
    }
    // vSemaphoreDelete(print_mux);
    // vTaskDelete(NULL);
}
/*************************************************INA3221**********************************************************/

void i2c_senor_task_create(void)
{
    ESP_ERROR_CHECK(i2c_master_init()); // Initialize I2C master

    /*Test I2C communication in parallel. Each task independently communicates with the BH1750 light intensity sensor, reads the data and prints the results. */
    xTaskCreate(i2c_BH1750_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
    xTaskCreate(i2c_SHT35_task, "i2c_test_task_1", 1024 * 2, (void *)0, 10, NULL);   // Create first I2C test task
    // xTaskCreate(i2c_MCP4725_task, "i2c_test_task_2", 1024 * 2, (void *)0, 10, NULL); // Create first I2C test task
    // xTaskCreate(i2c_INA3221_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL); // Create first I2C test task
}
