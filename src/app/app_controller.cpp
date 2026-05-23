#include "app/app_controller.hpp"

#include "board/board_config.hpp"
#include "hal/board_hal.hpp"
#include "sync/usb_time_sync.hpp"
#include "ui/ui_calendar.hpp"
#include "ui/ui_clock.hpp"
#include "ui/ui_stopwatch.hpp"
#include "util/calendar.hpp"

#include "pico/stdlib.h"
#include <cstdio>

AppController::AppController() : oled_(BOARD_SSD1306_I2C, BOARD_SSD1306_I2C_ADDR) {}

bool AppController::init() {
    if (!board::init_i2c_buses()) {
        return false;
    }
    if (!board::init_display(&oled_)) {
        return false;
    }
    if (!board::init_rtc(&rtc_)) {
        return false;
    }

    keys_.init();

    ds3231_time_t now{};
    if (ds3231_read_time(&rtc_, &now)) {
        align_calendar_view(now);
        last_rtc_second_ = now.seconds;
    }

    bool osf = false;
    if (ds3231_is_oscillator_stopped(&rtc_, &osf) && osf) {
        show_boot_screen();
        oled_.clearDisplay();
        oled_.setCursor(0, 0);
        oled_.setTextSize(1);
        oled_.print("RTC needs sync");
        oled_.setCursor(0, 12);
        oled_.print("Use USB serial");
        oled_.display();
        sleep_ms(1500);
    } else {
        show_boot_screen();
    }

    usb_sync_print_banner();
    printf("PicoChrono: DS3231 I2C0 GP%d/GP%d  OLED I2C1 GP%d/GP%d\r\n", BOARD_DS3231_SDA_PIN,
           BOARD_DS3231_SCL_PIN, BOARD_SSD1306_SDA_PIN, BOARD_SSD1306_SCL_PIN);
    printf("Keys: UP=GP%d DOWN=GP%d OK=GP%d (active low)\r\n", BOARD_KEY_UP_PIN, BOARD_KEY_DOWN_PIN,
           BOARD_KEY_OK_PIN);
    usb_sync_print_prompt();

    clock_dirty_ = true;
    return true;
}

void AppController::show_boot_screen() {
    oled_.clearDisplay();
    oled_.setCursor(16, 24);
    oled_.setTextSize(1);
    oled_.print("PicoChrono");
    oled_.display();
    sleep_ms(800);
}

void AppController::show_error(const char* msg) {
    oled_.clearDisplay();
    oled_.setCursor(0, 24);
    oled_.setTextSize(1);
    oled_.print(msg);
    oled_.display();
}

void AppController::align_calendar_view(const ds3231_time_t& rtc_now) {
    view_year_ = util::full_year_from_rtc(rtc_now.year);
    view_month_ = rtc_now.month;
}

void AppController::cycle_mode() {
    switch (mode_) {
    case AppMode::Clock:
        mode_ = AppMode::Calendar;
        calendar_dirty_ = true;
        break;
    case AppMode::Calendar:
        mode_ = AppMode::Stopwatch;
        stopwatch_dirty_ = true;
        break;
    case AppMode::Stopwatch:
        mode_ = AppMode::Clock;
        clock_dirty_ = true;
        break;
    }
}

void AppController::shift_view_month(int delta) {
    int y = static_cast<int>(view_year_);
    int m = static_cast<int>(view_month_) + delta;
    while (m < 1) {
        m += 12;
        y -= 1;
    }
    while (m > 12) {
        m -= 12;
        y += 1;
    }
    if (y < 2000) {
        y = 2000;
    }
    if (y > 2099) {
        y = 2099;
    }
    view_year_ = static_cast<unsigned>(y);
    view_month_ = static_cast<unsigned>(m);
    calendar_dirty_ = true;
}

void AppController::on_key(const input::KeyEvent& ev, uint32_t now_ms) {
    if (ev.kind == input::KeyEventKind::None) {
        return;
    }

    switch (mode_) {
    case AppMode::Clock:
        if (ev.key == input::KeyId::Ok && ev.kind == input::KeyEventKind::ShortPress) {
            cycle_mode();
        }
        break;

    case AppMode::Calendar:
        if (ev.key == input::KeyId::Up && ev.kind == input::KeyEventKind::ShortPress) {
            shift_view_month(-1);
        } else if (ev.key == input::KeyId::Down && ev.kind == input::KeyEventKind::ShortPress) {
            shift_view_month(+1);
        } else if (ev.key == input::KeyId::Ok && ev.kind == input::KeyEventKind::ShortPress) {
            cycle_mode();
        }
        break;

    case AppMode::Stopwatch:
        if (ev.key == input::KeyId::Ok) {
            if (ev.kind == input::KeyEventKind::LongPress) {
                stopwatch_.reset();
                stopwatch_dirty_ = true;
            } else if (ev.kind == input::KeyEventKind::ShortPress) {
                stopwatch_.toggle(now_ms);
                stopwatch_dirty_ = true;
            }
        } else if (ev.key == input::KeyId::Up && ev.kind == input::KeyEventKind::ShortPress) {
            mode_ = AppMode::Clock;
            clock_dirty_ = true;
        }
        break;
    }
}

void AppController::poll_usb_sync() {
    ds3231_time_t synced{};
    const int rc = usb_sync_poll_line(&rtc_, &synced);
    if (rc == 1) {
        align_calendar_view(synced);
        last_rtc_second_ = synced.seconds;
        clock_dirty_ = true;
        calendar_dirty_ = (mode_ == AppMode::Calendar);
    }
}

void AppController::poll_rtc_second() {
    ds3231_time_t now{};
    if (!ds3231_read_time(&rtc_, &now)) {
        return;
    }
    if (now.seconds != last_rtc_second_) {
        last_rtc_second_ = now.seconds;
        if (mode_ == AppMode::Clock) {
            clock_dirty_ = true;
        } else if (mode_ == AppMode::Calendar) {
            calendar_dirty_ = true;
        }
    }
}

void AppController::render(uint32_t now_ms) {
    switch (mode_) {
    case AppMode::Clock: {
        if (!clock_dirty_) {
            break;
        }
        ds3231_time_t now{};
        float temp = 0.0f;
        if (ds3231_read_time(&rtc_, &now)) {
            ds3231_read_temperature(&rtc_, &temp);
            ui::clock_paint(oled_, now, temp);
        }
        clock_dirty_ = false;
        break;
    }
    case AppMode::Calendar: {
        if (!calendar_dirty_) {
            break;
        }
        ds3231_time_t now{};
        if (ds3231_read_time(&rtc_, &now)) {
            ui::calendar_paint(oled_, view_year_, view_month_, now);
        }
        calendar_dirty_ = false;
        break;
    }
    case AppMode::Stopwatch: {
        const bool running = stopwatch_.state() == services::StopwatchState::Running;
        const bool due = stopwatch_dirty_ || (running && (now_ms - last_sw_paint_ms_ >= 50u));
        if (!due) {
            break;
        }
        ui::stopwatch_paint(oled_, stopwatch_, now_ms);
        stopwatch_dirty_ = false;
        last_sw_paint_ms_ = now_ms;
        break;
    }
    }
}

void AppController::tick(uint32_t now_ms) {
    poll_usb_sync();

    const input::KeyEvent ev = keys_.poll(now_ms);
    on_key(ev, now_ms);

    poll_rtc_second();
    render(now_ms);
}
