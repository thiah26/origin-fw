#include <Arduino.h>
#include <M5Unified.h>
#include "core/menu.h"
#include "core/ui.h"
#include "core/input.h"

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    Serial.begin(115200);

    randomSeed(esp_random());

    ui_init();
    menu_init();
    menu_draw();
}

void loop() {
    Button b = input_read();
    if (b == BTN_A_PRESS) menu_next();
    if (b == BTN_B_PRESS) menu_prev();
    if (b == BTN_C_PRESS) menu_select();
    delay(30);
}
