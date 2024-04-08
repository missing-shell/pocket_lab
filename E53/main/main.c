/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "bsp/esp32_s3_e53.h"
#include "lvgl.h"
#include "ui/ui.h"
#include "lcd_init.h"
#include "mytask_list.h"
#define TAG "ESP-EXAMPLE"

/*******************************************************************************
 * Private functions
 *******************************************************************************/

// *INDENT-OFF*
void app_lvgl_display(void)
{
    bsp_display_lock(0);

    ui_init();

    bsp_display_unlock();
}

static void core_task()
{
    while (1)
    {
        task_list();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init(); /* (NVS) Initialize the default NVS partition. */
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Initialize display and LVGL */
    // bsp_display_start();
    // e53_lcd_init();

    // /* Turn on display backlight */
    // bsp_display_backlight_on();

    /* Add and show objects on display */
    // app_lvgl_display();
    // ui_init();
    ESP_LOGI(TAG, "Example initialization done.");

    printf("-------------------yes------------------------------------\n");
    static void core_task();

    // ad_test();
    // da_test();
    // raise the task priority of LVGL and/or reduce the handler period can improve the performance
    vTaskDelay(pdMS_TO_TICKS(10));
    // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
    // lv_timer_handler();

    // extern void test_sample_init_task(void *pvParameters);
    // xTaskCreate(test_sample_init_task, "TestSampleInitTask", 4096, NULL, 2, NULL);

    // while (1)
    // {
    //     // raise the task priority of LVGL and/or reduce the handler period can improve the performance
    //     vTaskDelay(pdMS_TO_TICKS(10));
    //     // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
    //     lv_timer_handler();
    // }
}
