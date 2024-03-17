#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "hal/gpio_hal.h"
#include "esp_log.h"

#include "logic_analyzer_const_definition.h"
#include "logic_analyzer_pin_definition.h"
#include "logic_analyzer_hal.h"
#ifdef CONFIG_ANALYZER_USE_SUMP
#include "logic_analyzer_sump.h"
#endif
#ifdef CONFIG_ANALYZER_USE_WS
#include "logic_analyzer_ws_server.h"

#endif
// definition test sample
// 43-tx 44-rx
// output pin ledc example
#define LEDC_OUTPUT_IO (40)

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_4_BIT // Set duty resolution to
#define LEDC_DUTY (50)                 // Set duty to 50%.
#define LEDC_FREQUENCY (100000)        // Frequency in Hertz. Set frequency at 100 kHz

static void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 50%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void print_sample_buf(uint8_t *sample_buf, int samples)
{
    for (int i = 0; i < samples; i++)
    {
        ESP_LOGI("SAMPLE_BUF", "Sample %d: %u", i, sample_buf[i]);
    }
}

static void lcd_logic_analyzer_cb(uint8_t *sample_buf, int samples, int sample_rate, int channels)
{
    ESP_LOGI("LOGIC_ANALYZER", "Samples: %d, Sample Rate: %d Hz, Channels: %d", samples, sample_rate, channels);
    if (samples > 100)
    {
        samples = 100;
    }
    print_sample_buf(sample_buf, samples);

    printf("\n");
}

static logic_analyzer_config_t lcd_cfg = {
    .pin = {LA_PIN_0, LA_PIN_1, LA_PIN_2, LA_PIN_3, LA_PIN_4, LA_PIN_5, LA_PIN_6, LA_PIN_7, LA_PIN_8, LA_PIN_9, LA_PIN_10, LA_PIN_11, LA_PIN_12, LA_PIN_13, LA_PIN_14, LA_PIN_15},
    .pin_trigger = LA_PIN_TRIGGER,
    .trigger_edge = LA_PIN_EDGE,
    .number_of_samples = LA_SAMPLE_COUNT,
    .sample_rate = LA_SAMPLE_RATE,
    .number_channels = LA_ANALYZER_CHANNELS,
    .samples_to_psram = LA_ANALYZER_PSRAM,
    .meashure_timeout = LA_DEFAULT_TiMEOUT,
    .logic_analyzer_cb = lcd_logic_analyzer_cb};

void test_sample_init(void)
{
    // Set the LEDC peripheral configuration
    example_ledc_init();
    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
    // test_air();
    vTaskDelay(5);

#ifdef CONFIG_ANALYZER_USE_WS
    logic_analyzer_ws_server();

    esp_err_t ret = start_logic_analyzer(&lcd_cfg);
    if (ret)
    {
        ESP_LOGE("LCD", "Start lcd logic analyzer error %x", ret);
    }
    else
    {
        ESP_LOGI("LCD", "Start lcd logic analyzer OK");
    }

#endif
}

// 任务函数，用于调用 test_sample_init
void test_sample_init_task(void *pvParameters)
{
    // 调用 test_sample_init 函数
    test_sample_init();

    // 任务完成后，删除自己
    vTaskDelete(NULL);
}