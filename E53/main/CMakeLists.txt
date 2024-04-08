#Add sources from ui directory
file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "ui/*.c")
#Add sources from lab directory
file(GLOB_RECURSE SRC_LAB ${CMAKE_SOURCE_DIR} "lab/*.c")
idf_component_register( SRCS  
                            "main.c" 
                            "logic_analyzer_test.c"
                            ${SRC_UI}
                            ${SRC_LAB}
                        INCLUDE_DIRS 
                            "." "ui" "lab/include"
                        REQUIRES task_list logic_analyzer lvgl esp32_s3_e5
)
