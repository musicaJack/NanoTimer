#include "util/calendar.hpp"

namespace util {

unsigned full_year_from_rtc(uint8_t rtc_year) {
    return 2000u + static_cast<unsigned>(rtc_year % 100);
}

unsigned weekday_ds3231(unsigned y_full, unsigned mo, unsigned d) {
    int y = static_cast<int>(y_full);
    int m = static_cast<int>(mo);
    int day = static_cast<int>(d);
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (m < 3) {
        y -= 1;
    }
    int w = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
    if (w < 0) {
        w += 7;
    }
    return static_cast<unsigned>(w) + 1u;
}

bool is_leap_year(unsigned y) {
    if (y % 400u == 0u) {
        return true;
    }
    if (y % 100u == 0u) {
        return false;
    }
    return (y % 4u) == 0u;
}

unsigned days_in_month(unsigned y, unsigned m) {
    static const unsigned dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m < 1u || m > 12u) {
        return 31u;
    }
    if (m == 2u && is_leap_year(y)) {
        return 29u;
    }
    return dim[m - 1];
}

}  // namespace util
