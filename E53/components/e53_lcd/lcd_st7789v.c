#include <stdlib.h>
#include <sys/cdefs.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"

static const char *TAG = "st7789v";

static esp_err_t panel_st7789v_del(esp_lcd_panel_t *panel);
static esp_err_t panel_st7789v_reset(esp_lcd_panel_t *panel);
static esp_err_t panel_st7789v_init(esp_lcd_panel_t *panel);
static esp_err_t panel_st7789v_draw_bitmap(esp_lcd_panel_t *panel, int x_start, int y_start, int x_end, int y_end, const void *color_data);
static esp_err_t panel_st7789v_invert_color(esp_lcd_panel_t *panel, bool invert_color_data);
static esp_err_t panel_st7789v_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y);
static esp_err_t panel_st7789v_swap_xy(esp_lcd_panel_t *panel, bool swap_axes);
static esp_err_t panel_st7789v_set_gap(esp_lcd_panel_t *panel, int x_gap, int y_gap);
static esp_err_t panel_st7789v_disp_on_off(esp_lcd_panel_t *panel, bool off);

typedef struct {
    esp_lcd_panel_t base;
    esp_lcd_panel_io_handle_t io;
    int reset_gpio_num;
    bool reset_level;
    int x_gap;
    int y_gap;
    uint8_t fb_bits_per_pixel;
    uint8_t madctl_val; // save current value of LCD_CMD_MADCTL register
    uint8_t colmod_cal; // save surrent value of LCD_CMD_COLMOD register
} st7789v_panel_t;

esp_err_t esp_lcd_new_panel_st7789v(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel)
{
    esp_err_t ret = ESP_OK;
    st7789v_panel_t *st7789v = NULL;
    ESP_GOTO_ON_FALSE(io && panel_dev_config && ret_panel, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    st7789v = calloc(1, sizeof(st7789v_panel_t));
    ESP_GOTO_ON_FALSE(st7789v, ESP_ERR_NO_MEM, err, TAG, "no mem for st7789v panel");

    if (panel_dev_config->reset_gpio_num >= 0) {
        gpio_config_t io_conf = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << panel_dev_config->reset_gpio_num,
        };
        ESP_GOTO_ON_ERROR(gpio_config(&io_conf), err, TAG, "configure GPIO for RST line failed");
    }

    switch (panel_dev_config->color_space) {
    case ESP_LCD_COLOR_SPACE_RGB:
        st7789v->madctl_val = 0;
        break;
    case ESP_LCD_COLOR_SPACE_BGR:
        st7789v->madctl_val |= LCD_CMD_BGR_BIT;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported color space");
        break;
    }

    uint8_t fb_bits_per_pixel = 0;
    switch (panel_dev_config->bits_per_pixel) {
    case 16: // RGB565
        st7789v->colmod_cal = 0x55;
        fb_bits_per_pixel = 16;
        break;
    case 18: // RGB666
        st7789v->colmod_cal = 0x66;
        // each color component (R/G/B) should occupy the 6 high bits of a byte, which means 3 full bytes are required for a pixel
        fb_bits_per_pixel = 24;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported pixel width");
        break;
    }

    st7789v->io = io;
    st7789v->fb_bits_per_pixel = fb_bits_per_pixel;
    st7789v->reset_gpio_num = panel_dev_config->reset_gpio_num;
    st7789v->reset_level = panel_dev_config->flags.reset_active_high;
    st7789v->base.del = panel_st7789v_del;
    st7789v->base.reset = panel_st7789v_reset;
    st7789v->base.init = panel_st7789v_init;
    st7789v->base.draw_bitmap = panel_st7789v_draw_bitmap;
    st7789v->base.invert_color = panel_st7789v_invert_color;
    st7789v->base.set_gap = panel_st7789v_set_gap;
    st7789v->base.mirror = panel_st7789v_mirror;
    st7789v->base.swap_xy = panel_st7789v_swap_xy;
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
    st7789v->base.disp_off = panel_st7789v_disp_on_off;
#else
    st7789v->base.disp_on_off = panel_st7789v_disp_on_off;
#endif
    *ret_panel = &(st7789v->base);
    ESP_LOGD(TAG, "new st7789v panel @%p", st7789v);

    return ESP_OK;

err:
    if (st7789v) {
        if (panel_dev_config->reset_gpio_num >= 0) {
            gpio_reset_pin(panel_dev_config->reset_gpio_num);
        }
        free(st7789v);
    }
    return ret;
}

static esp_err_t panel_st7789v_del(esp_lcd_panel_t *panel)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);

    if (st7789v->reset_gpio_num >= 0) {
        gpio_reset_pin(st7789v->reset_gpio_num);
    }
    ESP_LOGD(TAG, "del st7789v panel @%p", st7789v);
    free(st7789v);
    return ESP_OK;
}

static esp_err_t panel_st7789v_reset(esp_lcd_panel_t *panel)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    esp_lcd_panel_io_handle_t io = st7789v->io;

    // perform hardware reset
    if (st7789v->reset_gpio_num >= 0) {
        gpio_set_level(st7789v->reset_gpio_num, st7789v->reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(st7789v->reset_gpio_num, !st7789v->reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
    } else { // perform software reset
        esp_lcd_panel_io_tx_param(io, LCD_CMD_SWRESET, NULL, 0);
        vTaskDelay(pdMS_TO_TICKS(20)); // spec, wait at least 5ms before sending new command
    }

    return ESP_OK;
}

typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t data_bytes; // Length of data in above data array; 0xFF = end of cmds.
} lcd_init_cmd_t;

static const lcd_init_cmd_t vendor_specific_init[] = {
    /* Memory Data Access Control, MX=MV=1, MY=ML=MH=0, RGB=0 */
    {0x36, {0x60}, 1},
    /* Interface Pixel Format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1},
    /* Porch Setting */
    {0xB2, {0x0c, 0x0c, 0x00, 0x33, 0x33}, 5},
    /* Gate Control, Vgh=13.65V, Vgl=-10.43V */
    {0xB7, {0x45}, 1},
    /* VCOM Setting, VCOM=1.175V */
    {0xBB, {0x2B}, 1},
    /* LCM Control, XOR: BGR, MX, MH */
    {0xC0, {0x2C}, 1},
    /* VDV and VRH Command Enable, enable=1 */
    {0xC2, {0x01, 0xff}, 2},
    /* VRH Set, Vap=4.4+... */
    {0xC3, {0x11}, 1},
    /* VDV Set, VDV=0 */
    {0xC4, {0x20}, 1},
    /* Frame Rate Control, 60Hz, inversion=0 */
    {0xC6, {0x0f}, 1},
    /* Power Control 1, AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V */
    {0xD0, {0xA4, 0xA1}, 1},
    /* Positive Voltage Gamma Control */
    {0xE0, {0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19}, 14},
    /* Negative Voltage Gamma Control */
    {0xE1, {0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19}, 14},
    /* Sleep Out */
    {0x11, {0}, 0x80},
    /* Display On */
    {0x29, {0}, 0x80},

    // /* Column Address Set, range 0-239 */
    // {0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
    // /* Row Address Set, range 0-279 */
    // {0x2B, {0x00, 0x00, 0x01, 0x1F}, 4},
    
    {0, {0}, 0xff}
};

static esp_err_t panel_st7789v_init(esp_lcd_panel_t *panel)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    esp_lcd_panel_io_handle_t io = st7789v->io;

    // LCD goes into sleep mode and display will be turned off after power on reset, exit sleep mode first
    esp_lcd_panel_io_tx_param(io, LCD_CMD_SLPOUT, NULL, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]) {
        st7789v->madctl_val,
    }, 1);
    esp_lcd_panel_io_tx_param(io, LCD_CMD_COLMOD, (uint8_t[]) {
        st7789v->colmod_cal,
    }, 1);

    // vendor specific initialization, it can be different between manufacturers
    // should consult the LCD supplier for initialization sequence code
    int cmd = 0;
    while (vendor_specific_init[cmd].data_bytes != 0xff) {
        esp_lcd_panel_io_tx_param(io, vendor_specific_init[cmd].cmd, vendor_specific_init[cmd].data, vendor_specific_init[cmd].data_bytes & 0x1F);
        cmd++;
    }

    return ESP_OK;
}

static esp_err_t panel_st7789v_draw_bitmap(esp_lcd_panel_t *panel, int x_start, int y_start, int x_end, int y_end, const void *color_data)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    assert((x_start < x_end) && (y_start < y_end) && "start position must be smaller than end position");
    esp_lcd_panel_io_handle_t io = st7789v->io;

    x_start += st7789v->x_gap;
    x_end += st7789v->x_gap;
    y_start += st7789v->y_gap;
    y_end += st7789v->y_gap;

    // define an area of frame memory where MCU can access
    esp_lcd_panel_io_tx_param(io, LCD_CMD_CASET, (uint8_t[]) {
        (x_start >> 8) & 0xFF,
        x_start & 0xFF,
        ((x_end - 1) >> 8) & 0xFF,
        (x_end - 1) & 0xFF,
    }, 4);
    esp_lcd_panel_io_tx_param(io, LCD_CMD_RASET, (uint8_t[]) {
        (y_start >> 8) & 0xFF,
        y_start & 0xFF,
        ((y_end - 1) >> 8) & 0xFF,
        (y_end - 1) & 0xFF,
    }, 4);
    // transfer frame buffer
    size_t len = (x_end - x_start) * (y_end - y_start) * st7789v->fb_bits_per_pixel / 8;
    esp_lcd_panel_io_tx_color(io, LCD_CMD_RAMWR, color_data, len);

    return ESP_OK;
}

static esp_err_t panel_st7789v_invert_color(esp_lcd_panel_t *panel, bool invert_color_data)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    esp_lcd_panel_io_handle_t io = st7789v->io;
    int command = 0;
    if (invert_color_data) {
        command = LCD_CMD_INVON;
    } else {
        command = LCD_CMD_INVOFF;
    }
    esp_lcd_panel_io_tx_param(io, command, NULL, 0);
    return ESP_OK;
}

static esp_err_t panel_st7789v_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    esp_lcd_panel_io_handle_t io = st7789v->io;
    if (mirror_x) {
        st7789v->madctl_val |= LCD_CMD_MX_BIT;
    } else {
        st7789v->madctl_val &= ~LCD_CMD_MX_BIT;
    }
    if (mirror_y) {
        st7789v->madctl_val |= LCD_CMD_MY_BIT;
    } else {
        st7789v->madctl_val &= ~LCD_CMD_MY_BIT;
    }
    esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]) {
        st7789v->madctl_val
    }, 1);
    return ESP_OK;
}

static esp_err_t panel_st7789v_swap_xy(esp_lcd_panel_t *panel, bool swap_axes)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    esp_lcd_panel_io_handle_t io = st7789v->io;
    if (swap_axes) {
        st7789v->madctl_val |= LCD_CMD_MV_BIT;
    } else {
        st7789v->madctl_val &= ~LCD_CMD_MV_BIT;
    }
    esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]) {
        st7789v->madctl_val
    }, 1);
    return ESP_OK;
}

static esp_err_t panel_st7789v_set_gap(esp_lcd_panel_t *panel, int x_gap, int y_gap)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    st7789v->x_gap = x_gap;
    st7789v->y_gap = y_gap;
    return ESP_OK;
}

static esp_err_t panel_st7789v_disp_on_off(esp_lcd_panel_t *panel, bool on_off)
{
    st7789v_panel_t *st7789v = __containerof(panel, st7789v_panel_t, base);
    esp_lcd_panel_io_handle_t io = st7789v->io;
    int command = 0;

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
    on_off = !on_off;
#endif

    if (on_off) {
        command = LCD_CMD_DISPON;
    } else {
        command = LCD_CMD_DISPOFF;
    }
    esp_lcd_panel_io_tx_param(io, command, NULL, 0);
    return ESP_OK;
}
