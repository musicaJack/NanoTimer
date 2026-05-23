#pragma once

#include "ds3231.h"
#include "ssd1306.h"

namespace ui {

/** Calendar view: compact 128x64 grid; invert RTC today. */
void calendar_paint(SSD1306& oled, unsigned view_year, unsigned view_month, const ds3231_time_t& rtc_now);

}  // namespace ui
