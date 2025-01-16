#pragma once

#include <Arduino.h>
#include "Globals.h"

#define every(interval)\
  static uint32_t __every__##interval = millis();\
  if(millis() - __every__##interval >= interval && (__every__##interval = millis()))



float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
float lerp(float a, float b, float f);
float calculateHypotenuse(float x, float y);

void drawWrappedStr(const char* text, int x, int y, int maxWidth, bool centerAlign, int lineSpacing);

void drawPageHeader(String breadcrumb, String pageName);
void drawStringButton(int x, int y, String icon, String label, const uint8_t* font);
void drawGlyphButton(int x, int y, String icon, const uint8_t* font, int glyph);
void drawGrid();
void drawScrollBar(int totalItems, int hoveredIndex);