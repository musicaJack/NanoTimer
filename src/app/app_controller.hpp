#pragma once

#include "ds3231.h"
#include "input/key_input.hpp"
#include "services/stopwatch.hpp"
#include "ssd1306.h"

enum class AppMode : uint8_t { Clock, Calendar, Stopwatch };

/** Application layer: mode state machine, key routing, display refresh. */
class AppController {
public:
    AppController();

    bool init();
    void tick(uint32_t now_ms);

    AppMode mode() const { return mode_; }

private:
    SSD1306 oled_;
    ds3231_t rtc_{};
    input::KeyInput keys_;
    services::Stopwatch stopwatch_;

    AppMode mode_ = AppMode::Clock;
    unsigned view_year_ = 2026;
    unsigned view_month_ = 1;
    uint8_t last_rtc_second_ = 255;
    uint32_t last_sw_paint_ms_ = 0;

    bool clock_dirty_ = true;
    bool calendar_dirty_ = true;
    bool stopwatch_dirty_ = true;

    void align_calendar_view(const ds3231_time_t& rtc_now);
    void cycle_mode();
    void shift_view_month(int delta);
    void on_key(const input::KeyEvent& ev, uint32_t now_ms);
    void poll_usb_sync();
    void poll_rtc_second();
    void render(uint32_t now_ms);
    void show_boot_screen();
    void show_error(const char* msg);
};
