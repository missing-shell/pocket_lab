#include "user_mqtt.h"

static const char *TAG = "MQTT_TASK";

char mqtt_publish_data1[] = "mqtt connect ok ";
char mqtt_publish_data2[] = "mqtt subscribe successful";
char mqtt_publish_data[512];

esp_mqtt_client_handle_t client;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    // your_context_t *context = event->context;
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, AliyunPublishTopic_user_update, mqtt_publish_data1, strlen(mqtt_publish_data1), 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        // msg_id = esp_mqtt_client_subscribe(client, AliyunSubscribeTopic_user_get, 0);
        // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, AliyunPublishTopic_user_update, mqtt_publish_data2, strlen(mqtt_publish_data2), 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    mqtt_event_handler_cb(event_data);
}

static void mqtt_test_task(void *pvParameters)
{
    uint8_t num = 0;

    while (1)
    {
        esp_mqtt_client_publish(client, AliyunPublishTopic_user_update, mqtt_publish_data, strlen(mqtt_publish_data), 1, 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void send_data()
{
    int LightLux = 0;
    float Humidity = 0;
    float temperature = 0;

    sprintf(mqtt_publish_data, "{\"id\": \"123\", \"version\": \"1.0\", \"sys\": {\"ack\": 1}, \"params\": {\"LightLux\": %d, \"Humidity\": %.2f, \"temperature\": %.2f}, \"method\": \"thing.event.property.post\"}", LightLux, Humidity, temperature);
}

void user_mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address = {
                .uri = MQTTHostUrl,
            }},
        .credentials = {.username = Aliyun_username, .client_id = Aliyun_client_id, .authentication = {
                                                                                        .password = Aliyun_password,
                                                                                        .use_secure_element = 1,
                                                                                    }},
    };

    send_data();

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    xTaskCreate(&mqtt_test_task, "mqtt_test_task", 4096, NULL, 5, NULL);
}
