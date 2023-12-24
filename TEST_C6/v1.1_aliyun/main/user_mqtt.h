#ifndef USER_MQTT_H
#define USER_MQTT_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "os.h"
/*********************
 *      DEFINES
 *********************/



#define MQTTHostUrl "mqtt://iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com:1883"
#define Aliyun_port 1883
#define Aliyun_client_id "esp32c6|securemode=2,signmethod=hmacmd5,timestamp=1702280032532|"
#define Aliyun_username "esp32c6&k0leyg2IxVc"
#define Aliyun_password "7EDBD088D6CA279ED7FC85010E3CD194"

#define AliyunSubscribeTopic_user_get "/sys/k0leyg2IxVc/esp32c6/thing/event/property/post_reply"
#define AliyunPublishTopic_user_update "/sys/k0leyg2IxVc/esp32c6/thing/event/property/post"

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    extern char mqtt_publish_data[512];
    void user_mqtt_app_start(void);

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif