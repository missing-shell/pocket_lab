#include "spi_test.h"

static const char *TAG = "SPI_TEST";
static SemaphoreHandle_t spi_mutex;
static spi_device_handle_t handle; // Declare a global variable to hold the SPI device handle
static int spi_state = -1;         //-1 未初始化  0 主设备  1   从设备

void spi_mutex_init(void)
{
    // Only create the mutex if it has not been created yet
    if (spi_mutex == NULL)
    {
        spi_mutex = xSemaphoreCreateMutex();
        if (spi_mutex == NULL)
        {
            // Log an error message
            ESP_LOGE(TAG, "Failed to create mutex for spi_master and handle");
            // Stop execution
            ESP_ERROR_CHECK(ESP_ERR_NO_MEM);
        }
    }
}
static void updata_rx_data(void *rx_data)
{
    lv_textarea_set_text(ui_spirxtext, (const char *)rx_data); // Set the text of the text area to the received rx_data
}

static void clean_ui_cb(void *rx_data)
{
    lv_textarea_set_text(ui_spitxtext, "");
    lv_textarea_set_text(ui_spirxtext, "");
}

/**********************************SPI-slave****************************************************/
void spi_slave_init(void)
{
    if (xSemaphoreTake(spi_mutex, 500) == pdTRUE)
    {
        spi_state = 1;
        xSemaphoreGive(spi_mutex);
    }

    e53_spi_slave_init();
}
/**********************************SPI-master****************************************************/
void spi_master_init(void)
{
    esp_err_t ret;

    if (xSemaphoreTake(spi_mutex, 500) == pdTRUE)
    {
        spi_state = 0;
        xSemaphoreGive(spi_mutex);
    }

    /*spi bus init*/
    e53_spi_init(); // master

    /*Configuration for the SPI device on the other side of the bus*/
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,           // Number of bits in command phase, set to 0 if not used
        .address_bits = 0,           // Number of bits in address phase, set to 0 if not used
        .dummy_bits = 0,             // Number of dummy bits in dummy phase, set to 0 if not used
        .clock_speed_hz = 10000000,  // Clock speed for SPI device, in Hz
        .duty_cycle_pos = 128,       // Duty cycle of clock, 128 means 50% duty cycle
        .mode = 0,                   // SPI mode (0, 1, 2, or 3)
        .spics_io_num = E53_SPI_NSS, // GPIO number for Chip Select (CS) line
        .cs_ena_posttrans = 3,       // Number of clock cycles to keep CS low after a transaction
        .queue_size = 3              // Size of queue for transactions
    };

    /*add the device we want to send stuff to.*/
    ret = spi_bus_add_device(E53_SPI_HOST, &devcfg, &handle);
    assert(ret == ESP_OK);
}

void spi_test(const char *tx_data)
{
    static char rx_data[SPI_TEST_BUF] = {0}; // Static to persist across function calls

    if (xSemaphoreTake(spi_mutex, 500) == pdTRUE)
    {
        if (!spi_state)
        {
            /*Declare a variable 'master_trans' of type 'spi_transaction_t' for SPI transactions*/
            spi_transaction_t master_trans;
            memset(&master_trans, 0, sizeof(master_trans));
            master_trans.length = 8 * SPI_TEST_BUF; // Transaction length is in bits
            master_trans.tx_buffer = tx_data;       // Data to send
            master_trans.rx_buffer = rx_data;       // Buffer to store received rx_data

            esp_err_t ret = spi_device_transmit(handle, &master_trans);
            if (ret != ESP_OK)
            {
                // Handle the error
                ESP_LOGE(TAG, "spi_master_transmit failed with error code: %d", ret);
            }
            else
            {
                ESP_LOGI(TAG, "Send str: %s", tx_data);
            }
        }
        else if (spi_state == 1)
        {
            /*Declare a variable 'master_trans' of type 'spi_transaction_t' for SPI transactions*/
            spi_slave_transaction_t slave_trans;
            memset(&slave_trans, 0, sizeof(slave_trans));
            slave_trans.length = 8 * SPI_TEST_BUF; // Transaction length is in bits
            slave_trans.tx_buffer = tx_data;       // Data to send
            slave_trans.rx_buffer = rx_data;       // Buffer to store received rx_data

            esp_err_t ret = spi_slave_transmit(E53_SPI_HOST, &slave_trans, 5000);
            if (ret != ESP_OK)
            {
                // Handle the error
                ESP_LOGE(TAG, "spi_slave_transmit failed with error code: %d", ret);
            }
            else
            {
                ESP_LOGI(TAG, "Send str: %s", tx_data);
            }
        }
        xSemaphoreGive(spi_mutex);
    }

    // Update the text area in the LVGL main thread
    lv_async_call((lv_async_cb_t)updata_rx_data, rx_data);
}

void clean_spiui(void)
{
    lv_async_call((lv_async_cb_t)clean_ui_cb, NULL);
    if (xSemaphoreTake(spi_mutex, 500) == pdTRUE)
    {
        // safely access spi_master and handle
        if (!spi_state)
        {
            spi_bus_remove_device(handle);
            e53_spi_deinit();
        }
        else if (spi_state == 1)
        {
            e53_spi_slave_deinit();
        }
        // give the mutex back when you are done
        xSemaphoreGive(spi_mutex);
    }
    else
    {
        // Handle the error
    }
}
