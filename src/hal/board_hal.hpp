#pragma once

#include "ds3231.h"
#include "ssd1306.h"

/** Board init: I2C buses, RTC, and OLED. */
namespace board {

bool init_i2c_buses();
bool init_rtc(ds3231_t* rtc);
bool init_display(SSD1306* oled);

}  // namespace board
