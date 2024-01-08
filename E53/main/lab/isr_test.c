#include "isr_test.h"

// 创建两个LED灯的对象
static lv_obj_t *led1;
static lv_obj_t *led2;
volatile int btn1_state;
volatile int btn2_state;

static void isr_timer_cb(lv_timer_t *timer)
{
    int level_1 = gpio_get_level(E53_IO_1);
    int level_2 = gpio_get_level(E53_IO_2);
    if (level_1 == 1)
    {
        lv_led_on(led1);
    }
    else
    {
        lv_led_off(led1);
    }

    if (level_2 == 1)
    {
        lv_led_on(led2);
    }
    else
    {
        lv_led_off(led2);
    }
}

static void led_ui(void)
{
    // 创建两个LED控件
    led1 = lv_led_create(ui_isrpanel);
    led2 = lv_led_create(ui_isrpanel);

    // 设置LED控件的颜色为绿色
    lv_color_t green = LV_COLOR_MAKE(0, 255, 0);
    lv_led_set_color(led1, green);
    lv_led_set_color(led2, green);

    // 设置LED控件的大小
    lv_obj_set_size(led1, 50, 50);
    lv_obj_set_size(led2, 50, 50);

    // 设置LED控件的位置
    lv_obj_set_align(led1, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_align(led2, LV_ALIGN_BOTTOM_RIGHT);

    // 初始化GPIO
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << E53_IO_1) | (1ULL << E53_IO_2), // LED引脚位掩码
        .mode = GPIO_MODE_INPUT,                                 // 设置为输入模式
        .pull_up_en = GPIO_PULLUP_ENABLE,                        // 启用上拉电阻
        .intr_type = GPIO_INTR_ANYEDGE                           // 设置为任意边沿触发中断
    };
    gpio_config(&led_config);
}

static void btn_ui(void)
{
    /*初始化按键GPIO*/
    esp_rom_gpio_pad_select_gpio(E53_IO_3);
    gpio_set_direction(E53_IO_3, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(E53_IO_4);
    gpio_set_direction(E53_IO_4, GPIO_MODE_OUTPUT);
}

void isr1_event_cb(lv_event_t *e)
{
    btn1_state = !btn1_state;             // 切换状态
    gpio_set_level(E53_IO_3, btn1_state); // 设置GPIO9的电平
}

void isr2_event_cb(lv_event_t *e)
{
    btn2_state = !btn2_state;             // 切换状态
    gpio_set_level(E53_IO_4, btn2_state); // 设置GPIO4的电平
}

void isr_test(void)
{
    led_ui();
    btn_ui();
    lv_timer_create(isr_timer_cb, 10, NULL);
}
