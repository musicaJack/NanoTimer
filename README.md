# NanoTimer (PicoChrono)

[![Platform](https://img.shields.io/badge/Platform-RP2040-brightgreen.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

[中文](README.zh.md) | English

PicoChrono is a compact RP2040 desktop clock on the **Waveshare RP2040-Zero**: **DS3231** RTC, **SSD1306** 128×64 OLED with dial switch, and USB time sync. Firmware modes: **clock**, **calendar**, **stopwatch**.

---

## Features

- DS3231 real-time clock (±2 ppm) with on-chip temperature
- OLED header (date / weekday / temp) + 7-segment `HH:MM:SS`
- Month calendar with today inverted
- Stopwatch with centiseconds while running
- USB CDC time sync to battery-backed RTC
- Debounced dial keys K1 / K2 / K3

## Hardware

### System diagram

```
                    ┌─────────────────────────┐
                    │   Waveshare RP2040-Zero │
                    └───────────┬─────────────┘
            I2C0 (GP4/GP5)    │    I2C1 (GP14/GP15)
                    ┌─────────┴─────────┐
              ┌─────▼─────┐      ┌──────▼──────┐
              │  DS3231   │      │ OLED + dial │
              └───────────┘      │ K1/K2/K3    │
                    │            └─────────────┘
              3.3V / GND            3.3V / GND
```

| Part | Spec |
|------|------|
| MCU | Waveshare RP2040-Zero (USB-C; WS2812 on GP16, unused) |
| RTC | DS3231, I2C **0x68**, CR2032 recommended |
| Display + input | 0.96" SSD1315/SSD1306 128×64, I2C **0x3C**; dial K1/K2/K3 with onboard pull-ups |

**Power:** all modules at **3.3 V**, common **GND**; never 5 V on logic pins. DS3231 keeps time on CR2032 when USB is unplugged.

**Wiring**

| Signal | GPIO | Bus / note |
|--------|------|------------|
| DS3231 SDA / SCL | GP4 / GP5 | I2C0 @ 100 kHz |
| OLED SDA / SCL | GP14 / GP15 | I2C1 @ 400 kHz |
| K1 / K2 / K3 (up / down / OK) | GP6 / GP7 / GP8 | Active low |

OLED module 7-pin order: **GND · VCC · SCL · SDA · K1 · K2 · K3**

Pin diagrams: [`docs/RP2040_Zero.png`](docs/RP2040_Zero.png), [`docs/OLED_SSD1306_0.96inch.png`](docs/OLED_SSD1306_0.96inch.png)

![OLED module](docs/OLED_SSD1306_0.96inch.png)

Firmware pins: `include/board/board_config.hpp` (`BOARD_DS3231_*`, `BOARD_SSD1306_*`, `BOARD_KEY_*`). When porting from `ssd1306` or `DS3231_Clock`, replace I2C instances and GPIO — do not copy their `#define` blocks.

**Display layout (128×64):** top ~20 px for date / weekday / temperature; main area for clock, calendar, or stopwatch. Clock uses 14×28 px 7-segment digits.

## Software

```
apps/picochrono/main.cpp     Entry, 10 ms loop
src/app/       AppController   Modes and routing
src/ui/        Views           clock / calendar / stopwatch
src/services/  Stopwatch
src/sync/      USB time sync   (usb_time_sync.cpp)
src/input/     Key debounce    (key_input.cpp)
src/hal/       Board init
src/util/      Calendar helpers
lib/ds3231, lib/ssd1306        Drivers
```

Each `AppController::tick()`: USB sync → keys → RTC second check → render if needed.

| Mode | Refresh | Keys |
|------|---------|------|
| Clock | RTC second | K3 → Calendar |
| Calendar | Second or month change | K1/K2 month; K3 → Stopwatch |
| Stopwatch | 50 ms when running | K3 start/pause; K3 long reset; K1 → Clock |

Keys: digital inputs (not quadrature); 30 ms debounce; short 20–900 ms; long ≥ 800 ms.

| Extension | Where |
|-----------|--------|
| New screen | `src/ui/` + `AppMode` |
| New peripheral | `lib/` + `src/hal/` |
| Pin change | `board_config.hpp` |

## Build & flash

Requires [Pico SDK](https://github.com/raspberrypi/pico-sdk) and `PICO_SDK_PATH`.

```batch
build_pico.bat
```

Or: `mkdir build && cd build && cmake -G Ninja .. && ninja` → flash `build/picochrono.uf2` via BOOTSEL.

## USB time sync

Non-blocking USB CDC parser in `src/sync/`. Send one line (115200) and press Enter:

| Format | Example |
|--------|---------|
| Time only | `14:30:00` |
| Date + time | `2026-05-23 14:30:00` |
| ISO | `2026-05-23T14:30:00` |

Year 2000–2099; valid calendar date. API: `usb_sync_poll_line(rtc, out)` → 1 OK, 0 pending, −1 error. If RTC OSF flag is set on first boot, sync before use.

## Drivers (quick reference)

**SSD1306** — `lib/ssd1306/`

```cpp
SSD1306 oled(i2c1, 0x3C);
oled.begin();
oled.setContrast(0x8F);
oled.clearDisplay();
oled.print("Hello");
oled.display();   // required to show on screen
```

If blank after draw: call `display()`. If `begin()` fails: check wiring and I2C init.

**DS3231** — `lib/ds3231/`

```c
ds3231_init(&rtc, i2c0, 4, 5);
ds3231_read_time(&rtc, &now);
```

Weekday field `day` is 1–7 (**1 = Sunday**); index arrays with `day - 1`. Refresh UI on RTC **second** changes, not software ticks. APIs: `ds3231_read_time`, `ds3231_write_time`, `ds3231_read_temperature`, `ds3231_is_oscillator_stopped`.

## External references

| Project | Use |
|---------|-----|
| `ssd1306` | OLED/RTC drivers, clock UI |
| `DS3231_Clock` | USB time sync |

## License

MIT — see [LICENSE](LICENSE).
