#include <stdio.h>  // Include the standard input/output library
#include <stdarg.h> // Include the standard argument library
#include <string.h>
#include "freertos/FreeRTOS.h" // Include the FreeRTOS library
#include "freertos/task.h"     // Include the FreeRTOS task library
#include "driver/uart.h"       // Include the ESP-IDF UART driver library
#include "driver/gpio.h"       // Include the ESP-IDF GPIO driver library
#include "sdkconfig.h"         // Include the SDK configuration file
#include "esp_log.h"           // Include the ESP-IDF log library
#include "driver/i2c.h"
#include "i2c_master.h"
#include "i2c_senor.h"
#include "user_mqtt.h"
#include "wifi_station.h"
#include "mytask_list.h"

// 创建全局互斥锁
SemaphoreHandle_t print_mutex = NULL;

int LightLux = -1;
float temperature = -1;
float Humidity = -1;

void i2c_consumer_task(void *pvParameter)
{
    SensorData received;
    while (1)
    {
        xQueueReceive(sensorQueue, &received, portMAX_DELAY);

        switch (received.type)
        {
        case DATA_TYPE_BH1750:
            LightLux = received.data.bh1750_lux;
            printf("LightLux=%d\n\n", LightLux);
            break;
        case DATA_TYPE_SHT35:
            temperature = received.data.sht35_val[0];
            Humidity = received.data.sht35_val[1];
            printf("temperature=%.4lf\n", temperature);
            printf(" Humidity=%.4lf\n", Humidity);
            break;
        case DATA_TYPE_INA3221:
            for (int i = 0; i < 3; i++)
            {
                printf("bus_val[%d]=%.4lf\n", i, received.data.ina3221.bus_val[i]);
                printf("shunt_val[%d]=%.4lf\n", i, received.data.ina3221.shunt_val[i]);
            }
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    // 初始化互斥锁
    print_mutex = xSemaphoreCreateMutex();

    // 确保互斥锁已成功创建
    if (print_mutex == NULL)
    {
        ESP_LOGE("app_main", "Failed to create mutex");
        return;
    }

    /*uart*/
    i2c_senor_init();
    station_init();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    while (1)
    {
        task_list();
        // sprintf(mqtt_publish_data, "{\"id\": \"123\", \"version\": \"1.0\", \"sys\": {\"ack\": 1}, \"params\": {\"LightLux\": %d, \"Humidity\": %.2f, \"Temperature\": %.2f}, \"method\": \"thing.event.property.post\"}",
        //         LightLux, Humidity, temperature);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
