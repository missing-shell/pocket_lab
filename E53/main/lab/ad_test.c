#include "ad_test.h"

static const char *TAG = "example";

typedef struct
{
    uint64_t event_count;
} example_queue_element_t;

lv_chart_series_t *ui_adchart_series_1;
static lv_coord_t ui_ad_val[500];

static void ad_init(void)
{
    ui_adchart_series_1 = lv_chart_add_series(ui_adchart, lv_palette_lighten(LV_PALETTE_GREEN, 2), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(ui_adchart, ui_adchart_series_1, ui_ad_val);
    // lv_chart_set_update_mode(ui_pwmchart, LV_CHART_UPDATE_MODE_CIRCULAR);
}

static bool IRAM_ATTR example_timer_on_alarm_cb_v3(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;          // 初始化高优先级任务被唤醒标志为false
    QueueHandle_t queue = (QueueHandle_t)user_data; // 获取队列句柄
    // 从队列中获取计数值并发送到队列
    example_queue_element_t ele = {
        .event_count = edata->count_value};
    xQueueSendFromISR(queue, &ele, &high_task_awoken); // 在中断服务程序中发送队列元素
    // 重新配置警报值
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = edata->alarm_value + 1000000, // 警报在下一秒
    };
    gptimer_set_alarm_action(timer, &alarm_config); // 设置警报动作
    // 返回是否需要在ISR结束时进行任务切换
    return (high_task_awoken == pdTRUE); // 如果高优先级任务被唤醒，则返回true
}
static void timer_init(void)
{
    example_queue_element_t ele;                                             // 定义队列元素
    QueueHandle_t queue = xQueueCreate(10, sizeof(example_queue_element_t)); // 创建一个队列
    if (!queue)
    {                                           // 如果队列创建失败
        ESP_LOGE(TAG, "Creating queue failed"); // 打印错误日志
        return;                                 // 返回
    }
    ESP_LOGI(TAG, "Create timer handle"); // 创建定时器句柄
    gptimer_handle_t gptimer = NULL;      // 定义定时器句柄
    gptimer_config_t timer_config = {
        // 定义定时器配置
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // 默认时钟源
        .direction = GPTIMER_COUNT_UP,      // 计数方向为向上
        .resolution_hz = 1000000,           // 分辨率为1MHz，1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); // 创建新的定时器

    gptimer_event_callbacks_t cbs = {
        // 定义定时器事件回调
        .on_alarm = example_timer_on_alarm_cb_v3 // 定义新的警报回调函数
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, queue)); // 注册新的定时器事件回调
    ESP_LOGI(TAG, "Enable timer");                                           // 启用定时器
    ESP_ERROR_CHECK(gptimer_enable(gptimer));                                // 检查定时器是否启用

    ESP_LOGI(TAG, "Start timer, update alarm value dynamically"); // 启动定时器，动态更新警报值
    gptimer_alarm_config_t alarm_config3 = {
        // 定义新的警报配置
        .alarm_count = 1000000, // 周期为1s
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config3)); // 设置新的警报动作
    ESP_ERROR_CHECK(gptimer_start(gptimer));                            // 启动定时器

    int record = 4; // 重新定义记录变量
    while (record)
    { // 当记录变量不为0时
        if (xQueueReceive(queue, &ele, pdMS_TO_TICKS(2000)))
        {                                                                // 如果在2000ms内接收到队列消息
            ESP_LOGI(TAG, "Timer alarmed, count=%llu", ele.event_count); // 打印定时器警报和计数值
            record--;                                                    // 记录变量减1
        }
        else
        {
            ESP_LOGW(TAG, "Missed one count event"); // 打印错过一个计数事件的警告
        }
    }

    ESP_LOGI(TAG, "Stop timer");                 // 停止定时器
    ESP_ERROR_CHECK(gptimer_stop(gptimer));      // 检查定时器是否停止
    ESP_LOGI(TAG, "Disable timer");              // 禁用定时器
    ESP_ERROR_CHECK(gptimer_disable(gptimer));   // 检查定时器是否禁用
    ESP_LOGI(TAG, "Delete timer");               // 删除定时器
    ESP_ERROR_CHECK(gptimer_del_timer(gptimer)); // 检查定时器是否删除

    vQueueDelete(queue); // 删除队列
}

void ad_test(void)
{
    ad_init();
    timer_init();
}
