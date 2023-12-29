#include "uart_test.h"

/*********************UART_RX*********************************/
static void uart_rx_timer_cb(lv_timer_t *timer)
{
    uint8_t *data = (uint8_t *)timer->user_data;

    if (data == NULL) // Only allocate memory if necessary
    {
        data = (uint8_t *)malloc(E53_BUF_SIZE + 1); // Allocate a buffer for the received data
        timer->user_data = data;
    }

    if (data == NULL) // If memory allocation failed...
    {
        ESP_LOGE("UART", "Failed to allocate memory for UART data");
        return;
    }

    int len = uart_read_bytes(E53_UART_NUM, data, E53_BUF_SIZE, 20 / portTICK_PERIOD_MS); // Read data from the UART

    if (len > 0) // If data was received...
    {
        data[len] = '\0'; // Null-terminate the received data
        char str[50];
        sprintf(str, "%s", data);
        ESP_LOGI("uart_rx_timer_cb", "Recev str: %s", str);
        lv_textarea_set_text(ui_rxtext, (const char *)data); // Set the text of the text area to the received data
    }
    else if (len < 0) // If an error occurred...
    {
        free(data);
        timer->user_data = NULL;
    }
}

void uart_test_init(void)
{
    e53_uart_init();
    lv_timer_create(uart_rx_timer_cb, 20, NULL);
}

/**********************************UART_TX********************************************************/
void uart_tx_test(const char *data)
{
    uart_write_bytes(E53_UART_NUM, data, strlen(data)); // 通过UART发送文本
    ESP_LOGI("uart_tx_test", "Send str: %s", (char *)data);
}
