#include "ui/ui_clock.hpp"

#include <cstdio>

namespace ui {

namespace {

constexpr int kHeaderHeight = 20;

const uint8_t kSegmentPatterns[10] = {
    0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011,
    0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011,
};

void draw_segment_digit(SSD1306& oled, int x, int y, int digit) {
    if (digit < 0 || digit > 9) {
        return;
    }

    const uint8_t pattern = kSegmentPatterns[digit];
    oled.fillRect(x, y, 14, 28, SSD1306_BLACK);

    if (pattern & 0b1000000) {
        oled.drawFastHLine(x + 1, y + 1, 12, SSD1306_WHITE);
    }
    if (pattern & 0b0100000) {
        oled.drawFastVLine(x + 12, y + 3, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0010000) {
        oled.drawFastVLine(x + 12, y + 15, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0001000) {
        oled.drawFastHLine(x + 1, y + 25, 12, SSD1306_WHITE);
    }
    if (pattern & 0b0000100) {
        oled.drawFastVLine(x + 1, y + 15, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0000010) {
        oled.drawFastVLine(x + 1, y + 3, 10, SSD1306_WHITE);
    }
    if (pattern & 0b0000001) {
        oled.drawFastHLine(x + 1, y + 13, 12, SSD1306_WHITE);
    }
}

void draw_colon(SSD1306& oled, int x, int y) {
    oled.fillCircle(x + 2, y + 8, 1, SSD1306_WHITE);
    oled.fillCircle(x + 2, y + 20, 1, SSD1306_WHITE);
}

void draw_header(SSD1306& oled, const ds3231_time_t& rtc, float temperature_c) {
    oled.fillRect(0, 0, 128, kHeaderHeight, SSD1306_BLACK);

    char date_str[16];
    std::snprintf(date_str, sizeof(date_str), "%04u-%02u-%02u", 2000u + rtc.year, rtc.month, rtc.date);
    oled.setCursor(2, 1);
    oled.setTextSize(1);
    oled.print(date_str);

    static const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const unsigned widx = (rtc.day >= 1u && rtc.day <= 7u) ? (rtc.day - 1u) : 0u;
    oled.setCursor(2, 9);
    oled.print(weekdays[widx]);

    char temp_str[12];
    std::snprintf(temp_str, sizeof(temp_str), "%.1fC", static_cast<double>(temperature_c));
    oled.setCursor(88, 9);
    oled.print(temp_str);
}

void draw_time(SSD1306& oled, const ds3231_time_t& rtc) {
    oled.fillRect(0, kHeaderHeight, 128, 64 - kHeaderHeight, SSD1306_BLACK);

    constexpr int kDigitPitch = 16;       // 14px digit + 2px gap
    constexpr int kGapAfterPair = 4;      // gap after digit pair before colon
    constexpr int kGapAfterColon = 6;     // gap after colon before next digit

    const int start_x = 4;                // was 14; shifted 10px left
    const int y = kHeaderHeight + 8;

    const int x_h1 = start_x;
    const int x_h2 = start_x + kDigitPitch;
    const int x_colon1 = start_x + 2 * kDigitPitch + kGapAfterPair;
    const int x_m1 = x_colon1 + kGapAfterColon;
    const int x_m2 = x_m1 + kDigitPitch;
    const int x_colon2 = x_m2 + 14 + kGapAfterPair;
    const int x_s1 = x_colon2 + kGapAfterColon;
    const int x_s2 = x_s1 + kDigitPitch;

    draw_segment_digit(oled, x_h1, y, rtc.hours / 10);
    draw_segment_digit(oled, x_h2, y, rtc.hours % 10);
    draw_colon(oled, x_colon1, y + 2);
    draw_segment_digit(oled, x_m1, y, rtc.minutes / 10);
    draw_segment_digit(oled, x_m2, y, rtc.minutes % 10);
    draw_colon(oled, x_colon2, y + 2);
    draw_segment_digit(oled, x_s1, y, rtc.seconds / 10);
    draw_segment_digit(oled, x_s2, y, rtc.seconds % 10);
}

}  // namespace

void clock_paint(SSD1306& oled, const ds3231_time_t& rtc, float temperature_c) {
    oled.clearDisplay();
    draw_header(oled, rtc, temperature_c);
    draw_time(oled, rtc);
    oled.display();
}

}  // namespace ui
