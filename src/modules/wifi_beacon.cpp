#include "wifi_beacon.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <M5Unified.h>
#include "../core/ui.h"
#include "../core/input.h"
#include "../boards/board_sticks3.h"

const char* REASON_SSIDS[] = {
    "Fsociety", "TFD", "You are loose", "Fuck u", "I'm loving it",
    "1488", "67", "69", "love sex", "uwu",
    "FBI Van", "NSA Free", "CIA Guest", "KGB WiFi", "MI6 Secure",
    "Hack Me", "No Password", "Trust Me", "Not A Trap", "Definitely Real",
    "Free Money", "Crypto Free", "Bitcoin Node", "Ethereum RPC", "Miner",
    "Rickroll", "Never Gonna", "Give You Up", "Never Gonna Let", "You Down",
    "404 Not Found", "500 Server Error", "403 Forbidden", "401 Unauthorized", "418 Teapot",
    "Loading...", "Please Wait", "Almost There", "Just Kidding", "Gotcha",
    "Samsung TV", "LG Smart", "Xiaomi Home", "TP-Link Admin", "D-Link Setup",
    "iPhone Hotspot", "Android AP", "iPad WiFi", "MacBook Pro", "AirDrop",
    "MTS Free", "Beeline Free", "Megafon Free", "Tele2 Free", "Yota Free",
    "Moscow Free", "SPB Free", "Kazan Free", "Sochi Free", "Any City Free",
    "Cafe WiFi", "Restaurant Guest", "Hotel Lobby", "Airport Free", "Train WiFi",
    "Metro Free", "Bus WiFi", "Taxi Free", "Park WiFi", "Mall Guest",
    "Library WiFi", "School Guest", "University Free", "Dorm WiFi", "Office Guest",
    "Home WiFi", "Home_2G", "Home_5G", "Family WiFi", "Neighbor WiFi",
    "Guest Network", "IoT Network", "Smart Home", "Alexa Setup", "Google Home",
    "Printer Setup", "HP-Print", "Canon WiFi", "Epson Direct", "Brother Setup",
    "Xbox Live", "PlayStation", "Nintendo WiFi", "Steam Link", "Game Server",
    "Netflix Free", "YouTube Free", "Spotify Free", "Twitch Free", "TikTok Free",
    "VPN Free", "Proxy Free", "Tor Exit", "Dark Web", "Onion Router",
    "Police WiFi", "Ambulance", "Fire Dept", "Emergency", "911 Free"
};
const int REASON_COUNT = sizeof(REASON_SSIDS) / sizeof(REASON_SSIDS[0]);

const char* RANDOM_PREFIX[] = {
    "Free", "Open", "Public", "Guest", "Home", "WiFi", "Net",
    "Link", "Node", "Hub", "Spot", "Zone", "Point", "Access"
};
const char* RANDOM_SUFFIX[] = {
    "WiFi", "Net", "Link", "Zone", "Spot", "Hub", "Node",
    "Point", "Access", "Connect", "Surf", "Web", "Online"
};

char random_ssids[50][32];

void generate_random_ssids() {
    for (int i = 0; i < 50; i++) {
        const char* p = RANDOM_PREFIX[random(14)];
        const char* s = RANDOM_SUFFIX[random(14)];
        int num = random(100, 9999);
        snprintf(random_ssids[i], 32, "%s_%s_%d", p, s, num);
    }
}

void wifi_beacon_send(const char* ssid, int channel) {
    uint8_t beacon[128] = {0};
    int len = 0;

    beacon[0] = 0x80; beacon[1] = 0x00;
    beacon[2] = 0x00; beacon[3] = 0x00;
    memset(&beacon[4], 0xFF, 6);
    for (int i = 0; i < 6; i++) beacon[10 + i] = random(256);
    memcpy(&beacon[16], &beacon[10], 6);
    beacon[22] = 0x00; beacon[23] = 0x00;
    memset(&beacon[24], 0x00, 8);
    beacon[32] = 0x64; beacon[33] = 0x00;
    beacon[34] = 0x01; beacon[35] = 0x04;

    int ssid_len = strlen(ssid);
    if (ssid_len > 32) ssid_len = 32;
    beacon[36] = 0x00;
    beacon[37] = ssid_len;
    memcpy(&beacon[38], ssid, ssid_len);
    len = 38 + ssid_len;

    beacon[len++] = 0x01; beacon[len++] = 0x08;
    beacon[len++] = 0x82; beacon[len++] = 0x84;
    beacon[len++] = 0x8B; beacon[len++] = 0x96;
    beacon[len++] = 0x0C; beacon[len++] = 0x12;
    beacon[len++] = 0x18; beacon[len++] = 0x24;

    beacon[len++] = 0x03; beacon[len++] = 0x01;
    beacon[len++] = (uint8_t)channel;

    esp_wifi_80211_tx(WIFI_IF_AP, beacon, len, false);
}

void beacon_reason_menu() {
    ui_message("Reason 100...");
    WiFi.mode(WIFI_AP);
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    unsigned long sent = 0;
    unsigned long last = millis();

    while (true) {
        for (int i = 0; i < REASON_COUNT; i++) {
            wifi_beacon_send(REASON_SSIDS[i], 1);
            delay(2);
            sent++;
        }
        if (millis() - last > 500) {
            ui_clear();
            ui_header("Reason (100)");
            M5.Display.setTextSize(2);
            M5.Display.setCursor(4, 40);
            M5.Display.printf("Sent: %lu", sent);
            M5.Display.setCursor(4, 70);
            M5.Display.setTextSize(1);
            M5.Display.print("C: stop");
            last = millis();
        }
        Button b = input_read();
        if (b == BTN_C_PRESS) break;
    }
    ui_message("Done");
    delay(500);
}

void beacon_random_menu() {
    ui_message("Reason Random...");
    generate_random_ssids();
    WiFi.mode(WIFI_AP);
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    int cycles = 0;
    unsigned long sent = 0;
    unsigned long last = millis();

    while (true) {
        for (int i = 0; i < 50; i++) {
            wifi_beacon_send(random_ssids[i], 1);
            delay(2);
            sent++;
        }
        if (++cycles >= 3) {
            cycles = 0;
            generate_random_ssids();
        }
        if (millis() - last > 500) {
            ui_clear();
            ui_header("Random (50)");
            M5.Display.setTextSize(2);
            M5.Display.setCursor(4, 40);
            M5.Display.printf("Sent: %lu", sent);
            M5.Display.setCursor(4, 70);
            M5.Display.setTextSize(1);
            M5.Display.print("C: stop");
            last = millis();
        }
        Button b = input_read();
        if (b == BTN_C_PRESS) break;
    }
    ui_message("Done");
    delay(500);
}
