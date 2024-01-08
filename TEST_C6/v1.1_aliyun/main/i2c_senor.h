#ifndef  I2C_SENOR_H
#define I2C_SENOR_H

#define DELAY_ITEMS_MS (1000)

/*sensorQueue*/
typedef enum {
    DATA_TYPE_BH1750,
    DATA_TYPE_SHT35,
    DATA_TYPE_INA3221,
} DataType;

typedef struct
{
    DataType type;
    union
    {
        int bh1750_lux;
        float sht35_val[2];
        struct
        {
            float bus_val[3];
            float shunt_val[3];
        } ina3221;
    } data;
} SensorData;

extern QueueHandle_t sensorQueue;


/*BH1750*/
#define BH1750_SENSOR_ADDR 0x23   /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START   0x23   /*!< Continuous high-resolution mode 2. The sensor continuously measures ambient light with high resolution (0.5 lx accuracy) for more precise measurements. */

/*SHT35_DIS_B*/
#define SHT35_DIS_B_SENSOR_ADDR 0x44
#define SHT35_DIS_B_CMD_START 0X2400

/*INA3221*/
#define INA3221_SENSOR_ADDR 0x40      // Define the I2C address for the INA3221 sensor
#define REG_BUS_VOLTAGE_CHANNEL1 0x02 // Register address for Bus Voltage
#define REG_BUS_VOLTAGE_CHANNEL2 0x04 // Register address for Bus Voltage of channel 2
#define REG_BUS_VOLTAGE_CHANNEL3 0x06 // Register address for Bus Voltage of channel 3
#define REG_SHUNT_VOLT_CHANNEL1 0x01  // Register address for Shunt Voltage of channel 1
#define REG_SHUNT_VOLT_CHANNEL2 0x03  // Register address for Shunt Voltage of channel 2
#define REG_SHUNT_VOLT_CHANNEL3 0x05  // Register address for Shunt Voltage of channel 3

void i2c_senor_init(void);

#endif