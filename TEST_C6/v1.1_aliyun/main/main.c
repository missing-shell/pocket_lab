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

void app_main(void)
{
    /*uart*/
    i2c_senor_task_create();
    station_init();
    
    
    while (1)
    {
        printf("----------------------\n");
    
        printf("bh1760_lux=%.4f\n\n", bh1760_lux);
        printf("sht35_val[0]=%.4lf\n", sht35_val[0]);
        printf("sht35_val[1]=%.4lf\n\n",sht35_val[1]);
        // printf("dac_val=%d\n\n",dac_val);
        // for(int i=0;i<3;i++)
        // {
        //     printf("bus_val[%d]=%.4lf\n",i,bus_val[i]);
        //     printf("shunt_val[%d]=%.4lf\n",i,shunt_val[i]);
        // }

        printf("\n");
        int LightLux = (int)bh1760_lux;
        float temperature = sht35_val[0];
        float Humidity = sht35_val[1];      
        sprintf(mqtt_publish_data, "{\"id\": \"123\", \"version\": \"1.0\", \"sys\": {\"ack\": 1}, \"params\": {\"LightLux\": %d, \"Humidity\": %.2f, \"Temperature\": %.2f}, \"method\": \"thing.event.property.post\"}", 
                                        LightLux, Humidity, temperature);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
