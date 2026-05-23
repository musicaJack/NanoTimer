#pragma once

#include <cstdint>

namespace util {

unsigned full_year_from_rtc(uint8_t rtc_year);
unsigned weekday_ds3231(unsigned year_full, unsigned month, unsigned day);
bool is_leap_year(unsigned year);
unsigned days_in_month(unsigned year, unsigned month);

}  // namespace util
