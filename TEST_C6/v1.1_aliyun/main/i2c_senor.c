#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include <math.h>
#include "i2c_master.h"
#include "i2c_senor.h"

static const char *BH1750_TAG = "BH1750";
static const char *SHT35_TAG = "SHT35_DIS_B";
static const char *INA3221_TAG = "INA3221";

QueueHandle_t sensorQueue;

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
static void i2c_BH1750_task(void *arg)
{
    int ret;

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
            // Read BH1750 data
            SensorData BH1750;
            BH1750.type = DATA_TYPE_BH1750;
            BH1750.data.bh1750_lux = (sensor_data_h << 8 | sensor_data_l) / 1.2;
            xQueueSend(sensorQueue, &BH1750, portMAX_DELAY);
        }
        else
        {
            ESP_LOGW(BH1750_TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret)); // Print warning message if no acknowledgement is received
        }
        vTaskDelay((DELAY_ITEMS_MS / portTICK_PERIOD_MS));
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
    uint8_t buff[6];
    while (1)
    {
        ret = i2c_master_sensor_SHT35(I2C_MASTER_NUM, buff); // Perform I2C sensor test
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
            uint16_t rawTemperature = (((uint16_t)buff[0] << 8) | buff[1]); // Combine the two bytes to get the raw temperature value
            uint16_t rawHumidity = (((uint16_t)buff[3] << 8) | buff[4]);    // Combine the two bytes to get the raw humidity value

            // Read SHT35 data
            SensorData SHT35;
            SHT35.type = DATA_TYPE_SHT35;
            SHT35.data.sht35_val[0] = -45 + 175 * ((double)rawTemperature / 65536); // Convert raw temperature to actual temperature
            SHT35.data.sht35_val[1] = 100 * ((double)rawHumidity / 65536);          // Convert raw humidity to actual humidity
            xQueueSend(sensorQueue, &SHT35, portMAX_DELAY);
        }
        else
        {
            ESP_LOGW(SHT35_TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret)); // Print warning message if no acknowledgement is received
        }
        vTaskDelay((DELAY_ITEMS_MS / portTICK_PERIOD_MS));
    }
    vTaskDelete(NULL);
}
/*************************************************SHT35**********************************************************/

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
        shunt_voltage[i] = (rawVoltage) * 0.00004; // Convert raw shunt voltage to actual voltage
        i2c_cmd_link_delete(cmd);                  // Always delete the cmd link after reading，
    }

    return ESP_OK;
}
static void i2c_INA3221_task(void *arg)
{

    int ret;
    double bus_voltage[3], shunt_voltage[3];
    double shunt_resistance = 100.0;
    double current[3];
    double power[3];
    while (1)
    {
        ret = i2c_master_sensor_test(I2C_MASTER_NUM, &bus_voltage, &shunt_voltage); // Call the new function to read the INA3221's data
        if (ret == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(INA3221_TAG, "I2C Timeout"); // Print error message if I2C timeout occurs
        }
        else if (ret == ESP_OK)
        {
            for (int i = 0; i < 3; i++)
            {
                // Read INA3221 data
                SensorData INA3221;
                INA3221.type = DATA_TYPE_INA3221;

                current[i] = shunt_voltage[i] / shunt_resistance;
                power[i] = bus_voltage[i] * current[i];
                INA3221.data.ina3221.bus_val[i] = bus_voltage[i];
                INA3221.data.ina3221.shunt_val[i] = shunt_voltage[i];
                xQueueSend(sensorQueue, &INA3221, portMAX_DELAY);
            }
        }
        else
        {
            ESP_LOGW(INA3221_TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret)); // Print warning message if no acknowledgement is received
        }
        vTaskDelay((DELAY_ITEMS_MS / portTICK_PERIOD_MS));
    }
    vTaskDelete(NULL);
}
/*************************************************INA3221**********************************************************/

void i2c_senor_init(void)
{
    ESP_ERROR_CHECK(i2c_master_init()); // Initialize I2C master

    sensorQueue = xQueueCreate(10, sizeof(SensorData));
    if (sensorQueue == NULL)
    {
        printf("Error creating the queue\n");
        ESP_LOGE("i2c_senor_init", "Error creating the queue"); 
    }

    // xTaskCreate(i2c_BH1750_task, "i2c_BH1750_task", 1024 * 2, (void *)0, 10, NULL);
    xTaskCreate(i2c_SHT35_task, "i2c_SHT35_task", 1024 * 2, (void *)0, 10, NULL);
    xTaskCreate(i2c_INA3221_task, "i2c_INA3221_task", 1024 * 2, (void *)0, 10, NULL);
}
