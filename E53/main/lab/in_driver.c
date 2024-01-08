#include "in_driver.h"

typedef struct
{
    lv_indev_drv_t drv;
    int pin;
} my_indev_drv_t;

bool read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    my_indev_drv_t *my_drv = (my_indev_drv_t *)drv;
    bool pin_state = gpio_get_level(my_drv->pin);                     // ��ȡGPIO���ŵ�״̬
    data->state = pin_state ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; // ������Ÿߵ�ƽ���򰴼������£����򣬰������ͷ�
    return false;                                                     // ����false��ʾû���µ������¼�
}

static void register_gpio(int pin)
{
    my_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv.drv);
    indev_drv.drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.drv.read_cb = read_cb;
    indev_drv.pin = pin;
    lv_indev_t *my_indev = lv_indev_drv_register((lv_indev_drv_t *)&indev_drv);
}