#include <stdio.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "esp_check.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/i2c.h"
#include "driver/uart.h"   
#include "bsp/esp32_s3_e53.h"
#include "bsp_err_check.h"
#include "bsp/display.h"
#include "esp_lvgl_port.h"

static const char *TAG = "S3-EYE";

static bool uart_initialized= false;
static bool e53_i2c_initialized = false;
static bool e53_spi_initialized= false;

static adc_oneshot_unit_handle_t e53_adc_handle = NULL;

/***********************GPIO*****************************/

/*******************button*****************************/


static const button_config_t bsp_button_config[BSP_BUTTON_NUM] = {
    {
        .type = BUTTON_TYPE_ADC,
        .adc_button_config.adc_handle = &e53_adc_handle,
        .adc_button_config.adc_channel = ADC_CHANNEL_0, // ADC1 channel 0 is GPIO1
        .adc_button_config.button_index = BSP_BUTTON_MENU,
        .adc_button_config.min = 2310, // middle is 2410mV
        .adc_button_config.max = 2510
    },
    {
        .type = BUTTON_TYPE_ADC,
        .adc_button_config.adc_handle = &e53_adc_handle,
        .adc_button_config.adc_channel = ADC_CHANNEL_0, // ADC1 channel 0 is GPIO1
        .adc_button_config.button_index = BSP_BUTTON_PLAY,
        .adc_button_config.min = 1880, // middle is 1980mV
        .adc_button_config.max = 2080
    },
    {
        .type = BUTTON_TYPE_ADC,
        .adc_button_config.adc_handle = &e53_adc_handle,
        .adc_button_config.adc_channel = ADC_CHANNEL_0, // ADC1 channel 0 is GPIO1
        .adc_button_config.button_index = BSP_BUTTON_DOWN,
        .adc_button_config.min = 720, // middle is 820mV
        .adc_button_config.max = 920
    },
    {
        .type = BUTTON_TYPE_ADC,
        .adc_button_config.adc_handle = &e53_adc_handle,
        .adc_button_config.adc_channel = ADC_CHANNEL_0, // ADC1 channel 0 is GPIO1
        .adc_button_config.button_index = BSP_BUTTON_UP,
        .adc_button_config.min = 280, // middle is 380mV
        .adc_button_config.max = 480
    }
};



/******************E53_ADC***********************/
esp_err_t e53_adc_initialize(void)
{
    /* ADC was initialized before */
    if (e53_adc_handle != NULL) {
        return ESP_OK;
    }

    /* Initialize ADC */
    const adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = BSP_ADC_UNIT,
    };
    BSP_ERROR_CHECK_RETURN_ERR(adc_oneshot_new_unit(&init_config1, &e53_adc_handle));

    return ESP_OK;
}

adc_oneshot_unit_handle_t e53_adc_get_handle(void)
{
    return e53_adc_handle;
}

/*****************E53 I2C*******************************/
esp_err_t e53_i2c_init(void)
{
    /* I2C was initialized before */
    if (e53_i2c_initialized) {
        return ESP_OK;
    }

    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = E53_I2C_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = E53_I2C_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = CONFIG_BSP_I2C_CLK_SPEED_HZ
    };
    BSP_ERROR_CHECK_RETURN_ERR(i2c_param_config(E53_I2C_NUM, &i2c_conf));
    BSP_ERROR_CHECK_RETURN_ERR(i2c_driver_install(E53_I2C_NUM, i2c_conf.mode, 0, 0, 0));

    e53_i2c_initialized = true;

    return ESP_OK;
}

esp_err_t e53_i2c_deinit(void)
{
    BSP_ERROR_CHECK_RETURN_ERR(i2c_driver_delete(E53_I2C_NUM));
    e53_i2c_initialized = false;
    return ESP_OK;
}


/************************E53 SPI*********************************/
esp_err_t e53_spi_init(void)
{
    /* SPI was initialized before */
    if (e53_spi_initialized) {
        return ESP_OK;
    }

    spi_bus_config_t buscfg3 = {
        .miso_io_num = E53_SPI_MISO,
        .mosi_io_num = E53_SPI_MOSI,
        .sclk_io_num = E53_SPI_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(E53_SPI_HOST, &buscfg3, SPI_DMA_CH_AUTO));

    e53_spi_initialized = true;

    return ESP_OK;
}
esp_err_t e53_spi_deinit(void)
{
    ESP_ERROR_CHECK(spi_bus_free(SPI3_HOST));
    e53_spi_initialized = false;
    return ESP_OK;
}



/*********************led**************************************/

esp_err_t bsp_leds_init(void)
{
    const gpio_config_t led_io_config = {
        .pin_bit_mask = BIT64(BSP_LED_GREEN),
        .mode = GPIO_MODE_OUTPUT_OD, // GPIO3 is connected directly to the LED (on board revision 2.1), so we use Open-drain here
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    BSP_ERROR_CHECK_RETURN_ERR(gpio_config(&led_io_config));
    return ESP_OK;
}

esp_err_t bsp_led_set(const bsp_led_t led_io, const bool on)
{
    BSP_ERROR_CHECK_RETURN_ERR(gpio_set_level((gpio_num_t) led_io, (uint32_t) on));
    return ESP_OK;
}

esp_err_t bsp_iot_button_create(button_handle_t btn_array[], int *btn_cnt, int btn_array_size)
{
    esp_err_t ret = ESP_OK;
    if ((btn_array_size < BSP_BUTTON_NUM) ||
            (btn_array == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }
 
    /* Initialize ADC and get ADC handle */
    BSP_ERROR_CHECK_RETURN_NULL(e53_adc_initialize());
    e53_adc_handle = e53_adc_get_handle();
 
    if (btn_cnt) {
        *btn_cnt = 0;
    }
    for (int i = 0; i < BSP_BUTTON_NUM; i++) {
        btn_array[i] = iot_button_create(&bsp_button_config[i]);
        if (btn_array[i] == NULL) {
            ret = ESP_FAIL;
            break;
        }
        if (btn_cnt) {
            (*btn_cnt)++;
        }
    }
    return ret;
}


/**********************E53 UART*************************/
esp_err_t e53_uart_init(void)
{
    /* UART was initialized before */
    if (uart_initialized) {
        return ESP_OK;
    }

    const uart_config_t uart_cfg = {
        .baud_rate = E53_UART_BAUD_RATE,
        .data_bits = E53_UART_DATA_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = E53_UART_STOP_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0; // Define the interrupt allocation flags

    ESP_ERROR_CHECK(uart_driver_install(E53_UART_NUM, E53_BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(E53_UART_NUM, &uart_cfg));
    ESP_ERROR_CHECK(uart_set_pin(E53_UART_NUM, E53_UART_TX, E53_UART_RX, E53_UART_RTS, E53_UART_CTS));

    uart_initialized = true;

    return ESP_OK;
}

esp_err_t e53_uart_deinit(void)
{
    ESP_ERROR_CHECK(uart_driver_delete(E53_UART_NUM));
    uart_initialized = false;
    return ESP_OK;
}


/***********************uart_emnu***********************/
typedef struct
{
    uart_word_length_t data_bits;
    char *name;
} DataBitsEntry;

DataBitsEntry dataBitsTable[] = {
    {UART_DATA_5_BITS, "UART_DATA_5_BITS"},
    {UART_DATA_6_BITS, "UART_DATA_6_BITS"},
    {UART_DATA_7_BITS, "UART_DATA_7_BITS"},
    {UART_DATA_8_BITS, "UART_DATA_8_BITS"},
    {UART_DATA_BITS_MAX, "UART_DATA_BITS_MAX"}};

typedef struct
{
    uart_stop_bits_t stop_bits;
    char *name;
} StopBitsEntry;

StopBitsEntry stopBitsTable[] = {
    {UART_STOP_BITS_1, "UART_STOP_BITS_1"},
    {UART_STOP_BITS_1_5, "UART_STOP_BITS_1_5"},
    {UART_STOP_BITS_2, "UART_STOP_BITS_2"},
    {UART_STOP_BITS_MAX, "UART_STOP_BITS_MAX"}};

uart_word_length_t getDataBitsByName(char *name)
{
    for (int i = 0; i < sizeof(dataBitsTable) / sizeof(DataBitsEntry); i++)
    {
        if (strcmp(dataBitsTable[i].name, name) == 0)
        {
            return dataBitsTable[i].data_bits;
        }
    }
    // Handle error case where name is not found...
    return UART_DATA_BITS_MAX;
}

uart_stop_bits_t getStopBitsByName(char *name)
{
    for (int i = 0; i < sizeof(stopBitsTable) / sizeof(StopBitsEntry); i++)
    {
        if (strcmp(stopBitsTable[i].name, name) == 0)
        {
            return stopBitsTable[i].stop_bits;
        }
    }
    // Handle error case where name is not found...
    return UART_STOP_BITS_MAX;
}