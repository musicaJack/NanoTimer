#pragma once

#include "services/stopwatch.hpp"
#include "ssd1306.h"

namespace ui {

void stopwatch_paint(SSD1306& oled, const services::Stopwatch& sw, uint32_t now_ms);

}  // namespace ui
