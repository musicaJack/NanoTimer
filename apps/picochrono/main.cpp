#include "app/app_controller.hpp"

#include "pico/stdlib.h"
#include <cstdio>

int main() {
    stdio_init_all();
    sleep_ms(800);

    AppController app;
    if (!app.init()) {
        printf("PicoChrono init failed.\r\n");
        while (true) {
            tight_loop_contents();
        }
    }

    while (true) {
        const uint32_t now_ms = to_ms_since_boot(get_absolute_time());
        app.tick(now_ms);
        sleep_ms(10);
    }

    return 0;
}
