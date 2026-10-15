#include "ui.h"
#include <M5Unified.h>
#include "../boards/board_sticks3.h"

void ui_init() {
    M5.Display.setRotation(DISPLAY_ROTATION);
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void ui_clear() {
    M5.Display.fillScreen(TFT_BLACK);
}

void ui_header(const char* title) {
    M5.Display.fillRect(0, 0, DISPLAY_WIDTH, 20, TFT_WHITE);
    M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(4, 2);
    M5.Display.print(title);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void ui_line(int idx, const char* text, bool selected) {
    int y = 30 + idx * 22;
    M5.Display.setTextSize(2);
    M5.Display.setCursor(4, y);
    if (selected) {
        M5.Display.fillRect(0, y - 2, DISPLAY_WIDTH, 20, TFT_WHITE);
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.print("> ");
    } else {
        M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        M5.Display.print("  ");
    }
    M5.Display.print(text);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
}

void ui_footer(const char* text) {
    M5.Display.fillRect(0, DISPLAY_HEIGHT - 18, DISPLAY_WIDTH, 18, TFT_DARKGREY);
    M5.Display.setTextColor(TFT_WHITE, TFT_DARKGREY);
    M5.Display.setTextSize(1);
    M5.Display.setCursor(4, DISPLAY_HEIGHT - 14);
    M5.Display.print(text);
}

void ui_flush() {}

void ui_message(const char* msg) {
    ui_clear();
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(4, 60);
    M5.Display.print(msg);
}
