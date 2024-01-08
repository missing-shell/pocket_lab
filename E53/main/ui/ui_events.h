// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: timergui

#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C"
{
#endif

    void gpio_test_initcb(lv_event_t *e);
    void isr_test_initcb(lv_event_t *e);
    void pwm_test_initcb(lv_event_t *e);
    void uart_test_initcb(lv_event_t *e);
    void i2c_test_initcb(lv_event_t *e);
    void spi_test_initcb(lv_event_t *e);
    void backlight(lv_event_t *e);
    void volumesetting(lv_event_t *e);
    void isrTestCb_1(lv_event_t *e);
    void isrTestCb_2(lv_event_t *e);
    void timeTestCb(lv_event_t *e);
    void pwmTestCb(lv_event_t *e);
    void clean_uartuicb(lv_event_t *e);
    void uartKeyBoardTxCb(lv_event_t *e);
    void clean_i2cuicb(lv_event_t *e);
    void i2cKeyBoardTxCb(lv_event_t *e);
    void clean_spiuicb(lv_event_t *e);
    void spiKeyBoardTxCb(lv_event_t *e);
    void spi_maste_initcb(lv_event_t *e);
    void spi_slave_initcb(lv_event_t *e);
    void wifiscanf(lv_event_t *e);
    void wifistart(lv_event_t *e);
    void wificlose(lv_event_t *e);
    void wifisanf(lv_event_t *e);
    void connectwifi(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
