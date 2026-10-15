#include "input.h"
#include <M5Unified.h>

void input_init() {}

Button input_read() {
    M5.update();
    if (M5.BtnA.wasPressed()) return BTN_A_PRESS;
    if (M5.BtnB.wasPressed()) return BTN_B_PRESS;
    if (M5.BtnC.wasPressed()) return BTN_C_PRESS;
    return BTN_NONE;
}
