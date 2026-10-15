#include "menu.h"
#include "ui.h"
#include "../modules/wifi_scan.h"
#include "../modules/wifi_deauth.h"
#include "../modules/wifi_beacon.h"

MenuItem menu[MENU_ITEMS] = {
    {"Wi-Fi Scan",          wifi_scan_menu},
    {"Wi-Fi Deauth",        wifi_deauth_menu},
    {"Reason (100)",        beacon_reason_menu},
    {"Reason Random (50)",  beacon_random_menu},
};

int menu_selected = 0;

void menu_init() { menu_selected = 0; }
void menu_next() { menu_selected = (menu_selected + 1) % MENU_ITEMS; menu_draw(); }
void menu_prev() { menu_selected = (menu_selected - 1 + MENU_ITEMS) % MENU_ITEMS; menu_draw(); }
void menu_select() { if (menu[menu_selected].action) menu[menu_selected].action(); }

void menu_draw() {
    ui_clear();
    ui_header("Origin");
    for (int i = 0; i < MENU_ITEMS; i++) {
        bool sel = (i == menu_selected);
        ui_line(i, menu[i].name, sel);
    }
    ui_footer("A:next  B:prev  C:ok");
    ui_flush();
}
