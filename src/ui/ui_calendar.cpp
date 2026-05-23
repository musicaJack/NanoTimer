#include "ui/ui_calendar.hpp"

#include "util/calendar.hpp"

#include <cstdio>
#include <cstring>

namespace ui {

namespace {

constexpr int kCellW = 18;
constexpr int kCellH = 7;
constexpr int kGridTop = 18;

void draw_day_cell(SSD1306& oled, int col, int row, int day_num, bool highlight) {
    const int x = col * kCellW + 1;
    const int y = kGridTop + row * kCellH;

    if (highlight) {
        oled.fillRect(x, y, kCellW - 2, kCellH - 1, SSD1306_WHITE);
        oled.setTextColor(SSD1306_BLACK);
    } else {
        oled.fillRect(x, y, kCellW - 2, kCellH - 1, SSD1306_BLACK);
        oled.setTextColor(SSD1306_WHITE);
    }

    char buf[4];
    std::snprintf(buf, sizeof(buf), "%d", day_num);
    oled.setCursor(x + 4, y);
    oled.setTextSize(1);
    oled.print(buf);
}

}  // namespace

void calendar_paint(SSD1306& oled, unsigned view_year, unsigned view_month, const ds3231_time_t& rtc_now) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);

    char title[12];
    std::snprintf(title, sizeof(title), "%04u-%02u", view_year, view_month);
    oled.setCursor(40, 0);
    oled.print(title);

    static const char* dow = "SMTWTFS";
    for (int c = 0; c < 7; ++c) {
        char label[2] = {dow[c], '\0'};
        oled.setCursor(c * kCellW + 5, 9);
        oled.print(label);
    }

    const unsigned dim = util::days_in_month(view_year, view_month);
    const unsigned wd1 = util::weekday_ds3231(view_year, view_month, 1u);
    const int start_skip = static_cast<int>(wd1 - 1u);

    const unsigned rtc_y = util::full_year_from_rtc(rtc_now.year);
    const unsigned rtc_m = rtc_now.month;
    const unsigned rtc_d = rtc_now.date;

    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            const int idx = row * 7 + col;
            const int day_num = idx - start_skip + 1;
            if (day_num < 1 || static_cast<unsigned>(day_num) > dim) {
                continue;
            }

            const bool today =
                (view_year == rtc_y && view_month == rtc_m && static_cast<unsigned>(day_num) == rtc_d);
            draw_day_cell(oled, col, row, day_num, today);
        }
    }

    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 57);
    oled.print("Up/Dn: month  OK: clock");

    oled.display();
}

}  // namespace ui
