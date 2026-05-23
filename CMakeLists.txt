cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico CACHE STRING "Target board" FORCE)

include(pico_sdk_import.cmake)

project(nanotimer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib)

set(NANOTIMER_APP_SOURCES
    apps/picochrono/main.cpp
    src/app/app_controller.cpp
    src/hal/board_hal.cpp
    src/input/key_input.cpp
    src/sync/usb_time_sync.cpp
    src/services/stopwatch.cpp
    src/util/calendar.cpp
    src/ui/ui_clock.cpp
    src/ui/ui_calendar.cpp
    src/ui/ui_stopwatch.cpp
)

add_executable(picochrono ${NANOTIMER_APP_SOURCES})

target_include_directories(picochrono PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(picochrono PRIVATE
    ds3231
    ssd1306
    pico_stdlib
    hardware_gpio
    hardware_i2c
)

pico_enable_stdio_usb(picochrono 1)
pico_enable_stdio_uart(picochrono 0)

pico_add_extra_outputs(picochrono)
