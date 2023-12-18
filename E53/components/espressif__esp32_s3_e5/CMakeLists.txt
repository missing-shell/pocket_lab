idf_component_register(
    SRCS        
            "esp32_s3_e53.c"
            "esp32_s3_peripherals.c"
             ${SRC_VER}
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "priv_include"
    REQUIRES driver 
    PRIV_REQUIRES  esp_lcd
)
