#pragma once

#include <cstdint>

namespace services {

enum class StopwatchState : uint8_t { Stopped, Running, Paused };

/** Stopwatch based on system time (independent of RTC). */
class Stopwatch {
public:
    void reset();
    void toggle(uint32_t now_ms);
    uint32_t elapsed_ms(uint32_t now_ms) const;

    StopwatchState state() const { return state_; }

private:
    StopwatchState state_ = StopwatchState::Stopped;
    uint32_t base_ms_ = 0;
    uint32_t started_at_ms_ = 0;
};

}  // namespace services
