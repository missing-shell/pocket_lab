#include"mytask_list.h"
#include "freertos/semphr.h"

// 引用在 main.c 中声明的互斥锁
extern SemaphoreHandle_t print_mutex;

void task_list(void)
{
    char ptrTaskList[250];
    vTaskList(ptrTaskList);

    // 锁定互斥锁
    xSemaphoreTake(print_mutex, portMAX_DELAY);

    printf("*******************************************************\n");
    printf("Task            State   Prio    Stack    Num    Core   \n");
    printf("*******************************************************\n");
    printf(ptrTaskList);
    printf("*******************************************************\n");

    // 解锁互斥锁
    xSemaphoreGive(print_mutex);
}