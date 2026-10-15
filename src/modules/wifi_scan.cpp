#include "wifi_scan.h"
#include <WiFi.h>
#include <M5Unified.h>
#include "../core/ui.h"
#include "../core/input.h"
#include "../boards/board_sticks3.h"

void wifi_scan_menu() {
    ui_message("Scanning...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int n = WiFi.scanNetworks();
    if (n <= 0) { ui_message("No networks"); delay(1500); return; }

    ui_clear();
    ui_header("Wi-Fi Scan");
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    int y = 24;
    for (int i = 0; i < n && i < 15; i++) {
        M5.Display.setCursor(2, y);
        M5.Display.printf("%2d %-18s %d", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
        y += 12;
    }
    ui_footer("C: back");

    while (true) {
        Button b = input_read();
        if (b == BTN_C_PRESS) break;
        delay(50);
    }
    WiFi.scanDelete();
}
