#include "hal/board_hal.hpp"

#include "board/board_config.hpp"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

namespace board {

static void init_i2c_bus(i2c_inst_t* inst, uint sda, uint scl, uint baud) {
    i2c_init(inst, baud);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);
}

bool init_i2c_buses() {
    init_i2c_bus(BOARD_DS3231_I2C, BOARD_DS3231_SDA_PIN, BOARD_DS3231_SCL_PIN, BOARD_DS3231_I2C_HZ);
    init_i2c_bus(BOARD_SSD1306_I2C, BOARD_SSD1306_SDA_PIN, BOARD_SSD1306_SCL_PIN, BOARD_SSD1306_I2C_HZ);
    return true;
}

bool init_rtc(ds3231_t* rtc) {
    if (!rtc) {
        return false;
    }
    return ds3231_init(rtc, BOARD_DS3231_I2C, BOARD_DS3231_SDA_PIN, BOARD_DS3231_SCL_PIN);
}

bool init_display(SSD1306* oled) {
    if (!oled) {
        return false;
    }
    if (!oled->begin()) {
        return false;
    }
    oled->setContrast(0x8F);
    return true;
}

}  // namespace board
