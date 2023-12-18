/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_log.h"
#include "bsp/esp32_s3_e53.h"
#include "lvgl.h"
#include "ui/ui.h"
#include "lcd_init.h"

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

void app_main(void)
{
    /* Initialize display and LVGL */
    //bsp_display_start();
    e53_lcd_init();

    // /* Turn on display backlight */
    // bsp_display_backlight_on();

    /* Add and show objects on display */
    //app_lvgl_display();
    ui_init();

    ESP_LOGI(TAG, "Example initialization done.");
   
    printf("-------------------yes------------------------------------\n");
    
    while (1)
    {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
    }

}
 
