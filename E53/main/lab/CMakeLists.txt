SET(SOURCES 
    gpio_test.c
    isr_test.c
    pwm_test.c
    ad_test.c
    da_test.c
    uart_test.c
    spi_test.c
)

include_directories(include)

add_library(lab ${SOURCES})