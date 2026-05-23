#include "input/key_input.hpp"

#include "board/board_config.hpp"
#include "hardware/gpio.h"

namespace input {

void KeyInput::init() {
    const uint pins[] = {BOARD_KEY_UP_PIN, BOARD_KEY_DOWN_PIN, BOARD_KEY_OK_PIN};
    for (unsigned i = 0; i < static_cast<unsigned>(KeyId::Count); ++i) {
        channels_[i].pin = pins[i];
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }
}

bool KeyInput::read_down(uint pin) const {
#if BOARD_KEYS_ACTIVE_LOW
    return gpio_get(pin) == 0;
#else
    return gpio_get(pin) != 0;
#endif
}

KeyEvent KeyInput::poll(uint32_t now_ms) {
    KeyEvent ev{};

    for (unsigned i = 0; i < static_cast<unsigned>(KeyId::Count); ++i) {
        Channel& ch = channels_[i];
        const bool raw = read_down(ch.pin);

        if (raw != ch.raw_down) {
            ch.raw_down = raw;
            ch.last_change_ms = now_ms;
        }

        if ((now_ms - ch.last_change_ms) >= BOARD_KEY_DEBOUNCE_MS && raw != ch.stable_down) {
            ch.prev_stable_down = ch.stable_down;
            ch.stable_down = raw;

            if (ch.stable_down) {
                ch.down_since_ms = now_ms;
                ch.long_fired = false;
                ev.key = static_cast<KeyId>(i);
                ev.kind = KeyEventKind::Pressed;
                return ev;
            }

            const uint32_t held = now_ms - ch.down_since_ms;
            ev.key = static_cast<KeyId>(i);
            ev.kind = KeyEventKind::Released;

            if (!ch.long_fired && held >= BOARD_KEY_SHORT_MIN_MS && held <= BOARD_KEY_SHORT_MAX_MS) {
                ev.kind = KeyEventKind::ShortPress;
            }
            return ev;
        }

        if (ch.stable_down && !ch.long_fired && (now_ms - ch.down_since_ms) >= BOARD_KEY_LONG_MS) {
            ch.long_fired = true;
            ev.key = static_cast<KeyId>(i);
            ev.kind = KeyEventKind::LongPress;
            return ev;
        }
    }

    return ev;
}

}  // namespace input
