#include "ui/ui_stopwatch.hpp"

#include <cstdio>

namespace ui {

namespace {

const char* state_label(services::StopwatchState st) {
    switch (st) {
    case services::StopwatchState::Running:
        return "RUN";
    case services::StopwatchState::Paused:
        return "PAUSE";
    default:
        return "STOP";
    }
}

void format_elapsed(uint32_t ms, char* buf, size_t len) {
    const uint32_t total_cs = ms / 10u;
    const uint32_t cs = total_cs % 100u;
    const uint32_t total_s = total_cs / 100u;
    const uint32_t s = total_s % 60u;
    const uint32_t m = (total_s / 60u) % 60u;
    const uint32_t h = total_s / 3600u;

    if (h > 0u) {
        std::snprintf(buf, len, "%02u:%02u:%02u", h, m, s);
    } else {
        std::snprintf(buf, len, "%02u:%02u.%02u", m, s, cs);
    }
}

}  // namespace

void stopwatch_paint(SSD1306& oled, const services::Stopwatch& sw, uint32_t now_ms) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);

    oled.setCursor(0, 0);
    oled.print("Stopwatch");
    oled.setCursor(90, 0);
    oled.print(state_label(sw.state()));

    char elapsed[16];
    format_elapsed(sw.elapsed_ms(now_ms), elapsed, sizeof(elapsed));

    oled.setTextSize(2);
    oled.setCursor(8, 24);
    oled.print(elapsed);

    oled.setTextSize(1);
    oled.setCursor(0, 56);
    oled.print("OK: run/pause  Hold: reset");

    oled.display();
}

}  // namespace ui
