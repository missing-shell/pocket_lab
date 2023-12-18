#ifndef __st7789v__
#define __st7789v__

esp_err_t esp_lcd_new_panel_st7789v(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel);

#endif
