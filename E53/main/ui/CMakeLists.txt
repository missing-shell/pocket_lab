SET(SOURCES screens/ui_home.c
    screens/ui_gpioui.c
    screens/ui_isrui.c
    screens/ui_timeui.c
    screens/ui_adui.c
    screens/ui_pwmui.c
    screens/ui_daui.c
    screens/ui_uartui.c
    screens/ui_i2cui.c
    screens/ui_spiui.c
    screens/ui_setting.c
    screens/ui_datetimeui.c
    screens/ui_wifiset.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_astronaut1_png.c
    images/ui_img_astronaut10_png.c
    images/ui_img_astronaut11_png.c
    images/ui_img_astronaut12_png.c
    images/ui_img_astronaut13_png.c
    images/ui_img_astronaut14_png.c
    images/ui_img_astronaut2_png.c
    images/ui_img_astronaut3_png.c
    images/ui_img_astronaut4_png.c
    images/ui_img_astronaut5_png.c
    images/ui_img_astronaut6_png.c
    images/ui_img_astronaut7_png.c
    images/ui_img_astronaut8_png.c
    images/ui_img_astronaut9_png.c
    fonts/ui_font_misans16.c
    fonts/ui_font_symbols16.c)

add_library(ui ${SOURCES})
