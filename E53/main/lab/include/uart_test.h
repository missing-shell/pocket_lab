#ifndef UART_TEST_H
#define UART_TEST_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "esp_log.h"
#include "bsp/esp32_s3_e53.h"
#include "lvgl.h"
#include "ui/ui.h"
#include "driver/uart.h"
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    void uart_tx_test(const char *data);
    void uart_test_init(void);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif