#include <Arduino.h>
#include "Helpers.h"
#include "Globals.h"
#include "Screen.h"
#include "Inputs.h"

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float lerp(float a, float b, float f)
{
  return a * (1.0 - f) + (b * f);
}

float calculateHypotenuse(float x, float y)
{
  float result = sqrt(pow(x, 2) + pow(y, 2));
  return result;
}

void drawPageHeader(String breadcrumb, String pageName)
{
  /*Back Button*/
  drawStringButton(5, 5, "A", "", FONT_TEXT);

  u8g2.setFont(FONT_TINY_TEXT);
  u8g2.drawStr(13, 9, breadcrumb.c_str());
  int width = u8g2.getStrWidth(breadcrumb.c_str());

  u8g2.setFont(FONT_HEADER);
  u8g2.drawStr(width + 13, 9, pageName.c_str());

  u8g2.drawHLine(-1, 11, 130);
}

void drawStringButton(int x, int y, String icon, String label, const uint8_t* font)
{
  u8g2.setFont(FONT_TINY_TEXT);
  u8g2.drawStr(x - 1, y + 3, icon.c_str());
  u8g2.drawRFrame(x - 4, y - 4, 9, 9, 3);

  u8g2.setFont(font);
  u8g2.drawStr(x + 7, y + 4, label.c_str());
}

void drawGlyphButton(int x, int y, String icon, const uint8_t* font, int glyph)
{
  u8g2.setFont(FONT_TINY_TEXT);
  u8g2.drawStr(x - 1, y + 3, icon.c_str());
  u8g2.drawRFrame(x - 4, y - 4, 9, 9, 3);

  u8g2.setFont(font);
  u8g2.drawGlyph(x + 7, y + 4, glyph);
}

void drawGrid() {
    for (int x = 0; x < 128; x += 10) {
        for (int y = 0; y < 64; y += 10) {
            if (x % 50 == 0 && y % 50 == 0) {
                u8g2.drawBox(x - 1, y - 1, 3, 3); // Draw thicker dots at every 50 pixels
            } else {
                u8g2.drawPixel(x, y);
            }
        }
    }
}

void drawWrappedStr(const char* text, int x, int y, int maxWidth, bool centerAlign, int lineSpacing) {
  int lineHeight = u8g2.getMaxCharHeight() + lineSpacing;
  int cursorY = y;
  const char* wordStart = text;

  while (*wordStart) {
    int lineWidth = 0;
    const char* lineStart = wordStart;
    const char* wordEnd;

    // Buffer to hold each word temporarily
    char wordBuffer[50];  // Adjust size if needed

    // Calculate the width of the next line
    while (*wordStart) {
      // Find end of the current word
      wordEnd = wordStart;
      while (*wordEnd && *wordEnd != ' ') {
        wordEnd++;
      }

      // Copy the word into wordBuffer
      int wordLength = wordEnd - wordStart;
      strncpy(wordBuffer, wordStart, wordLength);
      wordBuffer[wordLength] = '\0';  // Null-terminate the string

      // Get word width
      int wordWidth = u8g2.getStrWidth(wordBuffer);

      // Check if adding this word would exceed the maxWidth
      if (lineWidth + wordWidth > maxWidth) {
        break;
      }

      // Add word width to line width and move to the next word
      lineWidth += wordWidth;
      if (*wordEnd == ' ') {
        lineWidth += u8g2.getStrWidth(" ");
        wordEnd++;
      }
      wordStart = wordEnd;
    }

    // Set starting X position for center alignment if enabled
    int cursorX = x;
    if (centerAlign) {
      cursorX = x + (maxWidth - lineWidth) / 2;
    }

    // Draw the line
    while (lineStart < wordStart) {
      wordEnd = lineStart;
      while (*wordEnd && *wordEnd != ' ') {
        wordEnd++;
      }

      int wordLength = wordEnd - lineStart;
      strncpy(wordBuffer, lineStart, wordLength);
      wordBuffer[wordLength] = '\0';

      // Draw word and move cursor
      u8g2.setCursor(cursorX, cursorY);
      u8g2.print(wordBuffer);
      cursorX += u8g2.getStrWidth(wordBuffer);

      if (*wordEnd == ' ') {
        cursorX += u8g2.getStrWidth(" ");
        wordEnd++;
      }
      lineStart = wordEnd;
    }

    // Move to the next line
    cursorY += lineHeight;
  }
}

void drawScrollBar(int totalItems, int hoveredIndex) {
    int minHeight = 4;
    int maxHeight = 40;
    int scrollBarHeight = map(totalItems, 4, 20, maxHeight, minHeight);
    scrollBarHeight = constrain(scrollBarHeight, minHeight, maxHeight);
    int scrollBarY = map(hoveredIndex, 0, totalItems - 1, 13, 64 - scrollBarHeight);
    u8g2.drawBox(0, scrollBarY, 2, scrollBarHeight);
}
