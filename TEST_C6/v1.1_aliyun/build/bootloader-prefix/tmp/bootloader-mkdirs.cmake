# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Espressif/v5.1.1/esp-idf/components/bootloader/subproject"
  "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader"
  "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix"
  "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix/tmp"
  "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix/src/bootloader-stamp"
  "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix/src"
  "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/All_Project/ESP32/pocker_lab/TEST_C6/v1.1_aliyun/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
