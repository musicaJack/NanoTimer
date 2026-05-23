#ifndef DS3231_H
#define DS3231_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// DS3231 I2C address
#define DS3231_I2C_ADDR 0x68

// DS3231 register addresses
#define DS3231_SECONDS_REG    0x00
#define DS3231_MINUTES_REG    0x01
#define DS3231_HOURS_REG      0x02
#define DS3231_DAY_REG        0x03
#define DS3231_DATE_REG       0x04
#define DS3231_MONTH_REG      0x05
#define DS3231_YEAR_REG       0x06
#define DS3231_ALARM1_SEC     0x07
#define DS3231_ALARM1_MIN     0x08
#define DS3231_ALARM1_HOUR    0x09
#define DS3231_ALARM1_DAY     0x0A
#define DS3231_ALARM2_MIN     0x0B
#define DS3231_ALARM2_HOUR    0x0C
#define DS3231_ALARM2_DAY     0x0D
#define DS3231_CONTROL_REG    0x0E
#define DS3231_STATUS_REG     0x0F
#define DS3231_AGING_REG      0x10
#define DS3231_TEMP_MSB       0x11
#define DS3231_TEMP_LSB       0x12

// Control register bits
#define DS3231_EOSC_BIT       7
#define DS3231_BBSQW_BIT      6
#define DS3231_CONV_BIT       5
#define DS3231_RS2_BIT        4
#define DS3231_RS1_BIT        3
#define DS3231_INTCN_BIT      2
#define DS3231_A2IE_BIT       1
#define DS3231_A1IE_BIT       0

// Status register bits
#define DS3231_OSF_BIT        7
#define DS3231_EN32KHZ_BIT    3
#define DS3231_BSY_BIT        2
#define DS3231_A2F_BIT        1
#define DS3231_A1F_BIT        0

// Time structure
typedef struct {
    uint8_t seconds;   // 0-59
    uint8_t minutes;   // 0-59
    uint8_t hours;     // 0-23 (24-hour)
    uint8_t day;       // 1-7 (1=Sunday)
    uint8_t date;      // 1-31
    uint8_t month;     // 1-12
    uint8_t year;      // 0-99 (offset from 2000)
} ds3231_time_t;

// DS3231 device handle
typedef struct {
    i2c_inst_t *i2c;
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint8_t addr;
} ds3231_t;

// API
bool ds3231_init(ds3231_t *ds3231, i2c_inst_t *i2c, uint8_t sda_pin, uint8_t scl_pin);
bool ds3231_write_register(ds3231_t *ds3231, uint8_t reg, uint8_t value);
bool ds3231_read_register(ds3231_t *ds3231, uint8_t reg, uint8_t *value);
bool ds3231_read_time(ds3231_t *ds3231, ds3231_time_t *time);
bool ds3231_write_time(ds3231_t *ds3231, const ds3231_time_t *time);
bool ds3231_read_temperature(ds3231_t *ds3231, float *temperature);
bool ds3231_enable_oscillator(ds3231_t *ds3231, bool enable);
bool ds3231_is_oscillator_stopped(ds3231_t *ds3231, bool *stopped);
void ds3231_time_to_string(const ds3231_time_t *time, char *buffer, size_t buffer_size);

// Helpers
uint8_t bcd_to_bin(uint8_t bcd);
uint8_t bin_to_bcd(uint8_t bin);

#ifdef __cplusplus
}
#endif

#endif // DS3231_H
