#ifndef AD_TEST_H
#define AD_TEST_H

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
#include "driver/gptimer.h" // 包含定时器驱动库
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/

    void ad_test(void);
    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif