#pragma once

#include "pico/types.h"
#include <cstdint>

namespace input {

enum class KeyId : uint8_t { Up = 0, Down, Ok, Count };

enum class KeyEventKind : uint8_t { None, Pressed, Released, ShortPress, LongPress };

struct KeyEvent {
    KeyId key = KeyId::Up;
    KeyEventKind kind = KeyEventKind::None;
};

class KeyInput {
public:
    void init();
    /** Poll periodically; returns debounced key events. */
    KeyEvent poll(uint32_t now_ms);

private:
    struct Channel {
        uint pin = 0;
        bool raw_down = false;
        bool stable_down = false;
        bool prev_stable_down = false;
        uint32_t last_change_ms = 0;
        uint32_t down_since_ms = 0;
        bool long_fired = false;
    };

    Channel channels_[static_cast<unsigned>(KeyId::Count)]{};
    bool read_down(uint pin) const;
};

}  // namespace input
