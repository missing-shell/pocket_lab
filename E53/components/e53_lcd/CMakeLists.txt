 idf_component_register(SRCS
                        "lcd_init.c"
                        "lcd_st7789v.c"
                        ${SRC_VER}
                    INCLUDE_DIRS "include"
                    #PRIV_INCLUDE_DIRS "priv_include"
                    REQUIRES  driver lvgl
                    PRIV_REQUIRES esp_lcd
)
 

