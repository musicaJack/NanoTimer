#pragma once

/**
 * PicoChrono / NanoTimer — Waveshare RP2040-Zero pin and bus configuration.
 * Keep in sync with README wiring tables.
 */

#include "hardware/i2c.h"

// DS3231 RTC — I2C0
#define BOARD_DS3231_I2C      i2c0
#define BOARD_DS3231_SDA_PIN  4
#define BOARD_DS3231_SCL_PIN  5
#define BOARD_DS3231_I2C_HZ   100000u

// SSD1306 OLED — I2C1
#define BOARD_SSD1306_I2C     i2c1
#define BOARD_SSD1306_SDA_PIN 14
#define BOARD_SSD1306_SCL_PIN 15
#define BOARD_SSD1306_I2C_HZ  400000u
#define BOARD_SSD1306_I2C_ADDR 0x3C

// Dial keys (module pull-ups, active low)
#define BOARD_KEY_UP_PIN      6
#define BOARD_KEY_DOWN_PIN    7
#define BOARD_KEY_OK_PIN      8
#define BOARD_KEYS_ACTIVE_LOW 1

// Key timing (milliseconds)
#define BOARD_KEY_DEBOUNCE_MS     30u
#define BOARD_KEY_SHORT_MIN_MS    20u
#define BOARD_KEY_SHORT_MAX_MS    900u
#define BOARD_KEY_LONG_MS         800u
