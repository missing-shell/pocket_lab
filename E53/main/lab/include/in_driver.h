#ifndef IN_DRIVER_H
#define IN_DRIVER_H

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
#include "driver/gpio.h"
#include "driver/uart.h"
    /*********************
     *      DEFINES
     *********************/
/*GPIO*/
#define E53_IO_1 GPIO_NUM_1
#define E53_IO_2 GPIO_NUM_8
#define E53_IO_3 GPIO_NUM_9
#define E53_IO_4 GPIO_NUM_4
#define E53_IO_5 GPIO_NUM_7
    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
