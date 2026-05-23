#pragma once

#include "ds3231.h"
#include "ssd1306.h"

namespace ui {

/** Clock view: date/weekday/temp header + 7-segment HH:MM:SS. */
void clock_paint(SSD1306& oled, const ds3231_time_t& rtc, float temperature_c);

}  // namespace ui
