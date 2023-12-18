#ifndef __LCDK__
#define __LCDK__

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LCDK     GPIO_NUM_10
#define PIN_NUM_BK_LIGHT        10
#define LCD_BK_LIGHT_ON_LEVEL   1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL

void lcdk_control(void* arg);

void lcdk_task(void);

#endif