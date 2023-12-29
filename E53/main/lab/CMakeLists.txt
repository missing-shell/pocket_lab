SET(SOURCES 
    in_driver.c
    uart_test.c
)

include_directories(include)

add_library(lab ${SOURCES})