#ifndef __KEY__
#define __KEY__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define KEY GPIO_NUM_0
#define LCDK GPIO_NUM_10

void lcdk_init(void);
void key_scan_task(void* arg);

#endif