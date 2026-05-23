add_library(ds3231 STATIC ds3231/ds3231_driver.cpp)
target_include_directories(ds3231 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ds3231)
target_link_libraries(ds3231 PUBLIC pico_stdlib hardware_i2c hardware_gpio)

add_library(ssd1306 STATIC ssd1306/ssd1306.cpp)
target_include_directories(ssd1306 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ssd1306)
target_link_libraries(ssd1306 PUBLIC pico_stdlib hardware_i2c)
