
/**
 * @file
 * @brief ESP BSP: S3-EYE
 */

#pragma once

#include "sdkconfig.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "esp_codec_dev.h"
#include "iot_button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/spi_master.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "driver/sdmmc_host.h"

/**************************************************************************************************
 * ESP32-S3-EYE pinout
 **************************************************************************************************/

/* Display */
#define BSP_LCD_SPI_MOSI (GPIO_NUM_47)
#define BSP_LCD_SPI_CLK (GPIO_NUM_21)
#define BSP_LCD_SPI_CS (GPIO_NUM_44)
#define BSP_LCD_DC (GPIO_NUM_43)
#define BSP_LCD_RST (GPIO_NUM_NC)
#define BSP_LCD_BACKLIGHT (GPIO_NUM_48)

/*Touch*/
#define BSP_I2C_NUM (0) // CONFIG_BSP_I2C_NUM
#define BSP_I2C_SCL (GPIO_NUM_5)
#define BSP_I2C_SDA (GPIO_NUM_4)

/*GPIO*/
#define E53_IO_1 GPIO_NUM_1
#define E53_IO_2 GPIO_NUM_8
#define E53_IO_3 GPIO_NUM_9
#define E53_IO_4 GPIO_NUM_4
#define E53_IO_5 GPIO_NUM_7

/*AD/DA*/
#define BSP_ADC_UNIT ADC_UNIT_1
// #define E53_ADC  DAC      //
#define E53_DAC GPIO_NUM_2

/*I2C*/
#define E53_I2C_NUM (0)
#define E53_I2C_SCL (GPIO_NUM_41)
#define E53_I2C_SDA (GPIO_NUM_42)
#define BSP_I2C_NUM (CONFIG_BSP_I2C_NUM)

/*SPI*/
#define E53_SPI_HOST (SPI3_HOST)
#define SPI_TEST_BUF (128)
#define E53_SPI_SCK (GPIO_NUM_39)
#define E53_SPI_NSS (GPIO_NUM_40) // negative slave select
#define E53_SPI_MOSI (GPIO_NUM_15)
#define E53_SPI_MISO (GPIO_NUM_16)

/*UART*/
#define E53_UART_NUM (1)
#define E53_UART_RX (GPIO_NUM_5)
#define E53_UART_TX (GPIO_NUM_6)

#define E53_UART_RTS (UART_PIN_NO_CHANGE)
#define E53_UART_CTS (UART_PIN_NO_CHANGE)
#define E53_BUF_SIZE (1024)
#define E53_UART_BAUD_RATE (115200)
#define E53_UART_DATA_BITS UART_DATA_8_BITS
#define E53_UART_STOP_BITS UART_STOP_BITS_1

/* Others */
#define BSP_BUTTONS_IO (GPIO_NUM_1) // All 4 buttons mapped to this GPIO
typedef enum bsp_led_t
{
    BSP_LED_GREEN = GPIO_NUM_3,
} bsp_led_t;
/************************************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief BSP display configuration structure
     *
     */
    typedef struct
    {
        lvgl_port_cfg_t lvgl_port_cfg;
    } bsp_display_cfg_t;

/**************************************************************************************************
 *
 * LCD interface
 *
 * ESP32-S3-EYE is shipped with 1.3inch ST7789 display controller.
 * It features 16-bit colors and 240x240 resolution.
 *
 * LVGL is used as graphics library. LVGL is NOT thread safe, therefore the user must take LVGL mutex
 * by calling bsp_display_lock() before calling and LVGL API (lv_...) and then give the mutex with
 * bsp_display_unlock().
 **************************************************************************************************/
#define BSP_LCD_H_RES (240)
#define BSP_LCD_V_RES (240)
#define BSP_LCD_PIXEL_CLOCK_HZ (80 * 1000 * 1000)
#define BSP_LCD_SPI_NUM (SPI3_HOST)

#define BSP_LCD_DRAW_BUFF_SIZE (BSP_LCD_H_RES * BSP_LCD_V_RES)
#define BSP_LCD_DRAW_BUFF_DOUBLE (0)

    /**
     * @brief Initialize display
     *
     * This function initializes SPI, display controller and starts LVGL handling task.
     *
     * @return Pointer to LVGL display or NULL when error occured
     */
    lv_disp_t *bsp_display_start(void);

    /**
     * @brief Initialize display
     *
     * This function initializes SPI, display controller and starts LVGL handling task.
     * LCD backlight must be enabled separately by calling bsp_display_brightness_set()
     *
     * @param cfg display configuration
     *
     * @return Pointer to LVGL display or NULL when error occured
     */
    lv_disp_t *bsp_display_start_with_config(const bsp_display_cfg_t *cfg);

    /**
     * @brief Get pointer to input device (touch, buttons, ...)
     *
     * @note The LVGL input device is initialized in bsp_display_start() function.
     *
     * @return Pointer to LVGL input device or NULL when not initialized
     */
    lv_indev_t *bsp_display_get_input_dev(void);

    /**
     * @brief Take LVGL mutex
     *
     * @param timeout_ms Timeout in [ms]. 0 will block indefinitely.
     * @return true  Mutex was taken
     * @return false Mutex was NOT taken
     */
    bool bsp_display_lock(uint32_t timeout_ms);

    /**
     * @brief Give LVGL mutex
     *
     */
    void bsp_display_unlock(void);

    /**
     * @brief Set display's brightness
     *
     * Brightness is controlled with PWM signal to a pin controlling backlight.
     * Display must be already initialized by calling bsp_display_start()
     *
     * @param[in] brightness_percent Brightness in [%]
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   Parameter error
     */
    esp_err_t bsp_display_brightness_set(int brightness_percent);

    /**
     * @brief Turn on display backlight
     *
     * Display must be already initialized by calling bsp_display_start()
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   Parameter error
     */
    esp_err_t bsp_display_backlight_on(void);

    /**
     * @brief Turn off display backlight
     *
     * Display must be already initialized by calling bsp_display_start()
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   Parameter error
     */
    esp_err_t bsp_display_backlight_off(void);

    /**
     * @brief Rotate screen
     *
     * Display must be already initialized by calling bsp_display_start()
     *
     * @param[in] disp Pointer to LVGL display
     * @param[in] rotation Angle of the display rotation
     */
    void bsp_display_rotate(lv_disp_t *disp, lv_disp_rot_t rotation);

    /**************************************************************************************************
     *
     * Buttons interface
     *
     * Example configuration:
     * \code{.c}
     * button_handle_t btns[BSP_BUTTON_NUM];
     * bsp_iot_button_create(btns, NULL, BSP_BUTTON_NUM);
     * iot_button_register_cb(btns[0], ...
     * \endcode
     **************************************************************************************************/
    typedef enum
    {
        BSP_BUTTON_MENU = 0,
        BSP_BUTTON_DOWN,
        BSP_BUTTON_UP,
        BSP_BUTTON_PLAY,
        BSP_BUTTON_NUM
    } bsp_button_t;

    /**
     * @brief Initialize all buttons
     *
     * Returned button handlers must be used with espressif/button component API
     *
     * @note For LCD panel button which is defined as BSP_BUTTON_MAIN, bsp_display_start should
     *       be called before call this function.
     *
     * @param[out] btn_array      Output button array
     * @param[out] btn_cnt        Number of button handlers saved to btn_array, can be NULL
     * @param[in]  btn_array_size Size of output button array. Must be at least BSP_BUTTON_NUM
     * @return
     *     - ESP_OK               All buttons initialized
     *     - ESP_ERR_INVALID_ARG  btn_array is too small or NULL
     *     - ESP_FAIL             Underlaying iot_button_create failed
     */
    esp_err_t bsp_iot_button_create(button_handle_t btn_array[], int *btn_cnt, int btn_array_size);

    /**************************************************************************************************
     *
     * I2C interface
     *
     * There are two devices connected to I2C peripheral:
     *  - QMA7981 Inertial measurement unit
     *  - OV2640 Camera module
     **************************************************************************************************/

    /**
     * @brief Init I2C driver
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   I2C parameter error
     *      - ESP_FAIL              I2C driver installation error
     *
     */
    esp_err_t bsp_i2c_init(void);

    /**
     * @brief Deinit I2C driver and free its resources
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   I2C parameter error
     *
     */
    esp_err_t bsp_i2c_deinit(void);

    /**************************************************************************************************
     *
     * E53 I2C interface
     *
     *  -Initialize E53 I2C to communicate with other I2C devices
     *  -
     *  -
     **************************************************************************************************/

    /**
     * @brief Init E53 I2C driver
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   I2C parameter error
     *      - ESP_FAIL              I2C driver installation error
     *
     */
    esp_err_t e53_i2c_init(void);

    /**
     * @brief Deinit E53 I2C driver and free its resources
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   I2C parameter error
     *
     */
    esp_err_t e53_i2c_deinit(void);

    /**************************************************************************************************
     *
     * LEDs
     *
     **************************************************************************************************/

    /**
     * @brief Set LED's GPIOs as output push-pull
     *
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t bsp_leds_init(void);

    /**
     * @brief Turn LED on/off
     *
     * @param led_io LED io
     * @param on Switch LED on/off
     * @return
     *     - ESP_OK Success
     *     - ESP_ERR_INVALID_ARG Parameter error
     */
    esp_err_t bsp_led_set(const bsp_led_t led_io, const bool on);

    /**************************************************************************************************
     *
     * ADC interface
     *
     * There are multiple devices connected to ADC peripheral:
     *  - Buttons
     *
     * After initialization of ADC, use adc_handle when using ADC driver.
     **************************************************************************************************/

    /**
     * @brief Initialize E53 ADC
     *
     * The ADC can be initialized inside E53, when needed.
     *
     * @param[out] adc_handle Returned ADC handle
     */
    esp_err_t e53_adc_init(void);

    /**
     * @brief Get ADC handle
     *
     * @note This function is available only in IDF5 and higher
     *
     * @return ADC handle
     */
    adc_oneshot_unit_handle_t e53_adc_get_handle(void);

    /**************************************************************************************************
     *
     * E53 UART interface
     *
     *  -Initialize E53 UART to communicate with other UART devices
     *  -
     *  -
     **************************************************************************************************/

    /**
     * @brief Init E53 UART driver
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   UART parameter error
     *      - ESP_FAIL              UART driver installation error
     *
     */
    esp_err_t e53_uart_init(void);

    /**
     * @brief Deinit E53 UART driver and free its resources
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   UART parameter error
     *
     */
    esp_err_t e53_uart_deinit(void);

    /**************************************************************************************************
     *
     * E53 SPI interface
     *
     *  -Initialize E53 SPI to communicate with other SPI devices
     *  -
     *  -
     **************************************************************************************************/

    /**
     * @brief Init E53 SPI driver
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   SPI parameter error
     *      - ESP_FAIL              SPI driver installation error
     *
     */
    esp_err_t e53_spi_init(void);

    /**
     * @brief Deinit E53 I2C driver and free its resources
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   SPI parameter error
     *
     */
    esp_err_t e53_spi_deinit(void);

    /**************************************************************************************************
     *
     * E53 Slave SPI interface
     *
     *  -Initialize E53 Slave SPI to communicate with other SPI devices
     *  -
     *  -
     **************************************************************************************************/

    /**
     * @brief Init E53 Slave SPI driver
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   SPI parameter error
     *      - ESP_FAIL              SPI driver installation error
     *
     */
    esp_err_t e53_spi_slave_init(void);

    /**
     * @brief Deinit E53 I2C driver and free its resources
     *
     * @return
     *      - ESP_OK                On success
     *      - ESP_ERR_INVALID_ARG   SPI parameter error
     *
     */
    esp_err_t e53_spi_slave_deinit(void);

#ifdef __cplusplus
}
#endif
