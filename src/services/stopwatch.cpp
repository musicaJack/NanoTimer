#include "services/stopwatch.hpp"

namespace services {

void Stopwatch::reset() {
    state_ = StopwatchState::Stopped;
    base_ms_ = 0;
    started_at_ms_ = 0;
}

void Stopwatch::toggle(uint32_t now_ms) {
    if (state_ == StopwatchState::Running) {
        base_ms_ += now_ms - started_at_ms_;
        state_ = StopwatchState::Paused;
        return;
    }

    started_at_ms_ = now_ms;
    state_ = StopwatchState::Running;
}

uint32_t Stopwatch::elapsed_ms(uint32_t now_ms) const {
    if (state_ == StopwatchState::Stopped) {
        return 0;
    }
    if (state_ == StopwatchState::Running) {
        return base_ms_ + (now_ms - started_at_ms_);
    }
    return base_ms_;
}

}  // namespace services
