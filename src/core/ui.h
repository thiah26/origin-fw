#pragma once
#include <Arduino.h>

void ui_init();
void ui_clear();
void ui_header(const char* title);
void ui_line(int idx, const char* text, bool selected);
void ui_footer(const char* text);
void ui_flush();
void ui_message(const char* msg);
