#pragma once
#include <Arduino.h>

enum Button { BTN_NONE, BTN_A_PRESS, BTN_B_PRESS, BTN_C_PRESS };

void input_init();
Button input_read();
