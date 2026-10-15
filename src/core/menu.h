#pragma once
#include <Arduino.h>

#define MENU_ITEMS 4

struct MenuItem {
    const char* name;
    void (*action)();
};

extern MenuItem menu[MENU_ITEMS];
extern int menu_selected;

void menu_init();
void menu_next();
void menu_prev();
void menu_select();
void menu_draw();
