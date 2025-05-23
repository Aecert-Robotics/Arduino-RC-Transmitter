#include <Arduino.h>
#include "Page.h"
#include "Screen.h"
#include "Inputs.h"
#include "Globals.h"
#include "Popup.h"
#include "NRF.h"

enum SettingType
{
    BOOLEAN,
    INTEGER,
    STRING
};

struct Setting
{
    const char *name;        // Name of the setting
    SettingType type;        // Type of the setting
    void *value;             // Pointer to the value
    long int minVal, maxVal; // For integers or strings, define a range
};

uint8_t nrfAddress[EEPROM_NRF_ADDRESS_ARRAY_SIZE]; // NRF chip address as an array of bytes
long int numberTest;
bool booleanTest;

Setting settings[] = {
    {"NRF Addr", STRING, &nrfAddress, EEPROM_NRF_ADDRESS_ARRAY_SIZE - 1, EEPROM_NRF_ADDRESS_ARRAY_SIZE - 1},
    {"number test", INTEGER, &numberTest, -10, 10},
    {"boolean test", BOOLEAN, &booleanTest}};

const int numSettings = sizeof(settings) / sizeof(settings[0]);

void SettingsPage::init()
{
    rotaryEncoderButtonReady = false;
    rotaryEncoderSwitchValue = UNPRESSED;
}

void SettingsPage::loop()
{
    rotaryEncoderSwitchValue = getRotaryEncoderSwitchValue();

    if (rotaryEncoderSwitchValue == UNPRESSED)
        rotaryEncoderButtonReady = true;

    /*Back*/
    if (getButtonValue(A) == PRESSED)
    {
        currentPage = mainMenuPage;
    }

    /*Scrolling*/
    int increment = 0;
    int spins = getRotaryEncoderSpins();

    // limit to scrolling 1 item per frame
    if (spins > 0)
        increment = 1;
    if (spins < 0)
        increment = -1;

    // stop scrolling at ends of list
    hovered += increment;
    if (hovered >= numSettings)
        hovered = numSettings - 1;
    else if (hovered < 0)
        hovered = 0;

    /*Display Header*/
    drawPageHeader("< Home < Menu < ", "Settings");

    /*Display Debug*/
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(115, 63, String(getRotaryEncoderTotalSpins()).c_str());

    /*Display the list*/
    int rowSpacing = 14;
    int listYStart = 24;
    int listLeftSpacing = 7;

    if (hovered >= 2)
        listYStart = listYStart - rowSpacing * (hovered - 2);

    char buffer[32];

    u8g2.setFont(FONT_TEXT);
    for (int i = 0; i < numSettings; i++)
    {
        // Get the setting name and value pointer
        const char *name = settings[i].name;
        void *valuePtr = settings[i].value;

        // Convert the value based on its type
        if (settings[i].type == INTEGER)
        {
            // Format as a normal integer
            sprintf(buffer, "%s: %ld", name, *(long int *)valuePtr);
        }
        else if (settings[i].type == BOOLEAN)
        {
            // Format as "On" or "Off" for boolean settings
            sprintf(buffer, "%s: %s", name, (*(bool *)valuePtr) ? "On" : "Off");
        }
        else if (settings[i].type == STRING)
        {
            // Ensure the string is null-terminated
            char strBuffer[settings[i].maxVal]; // 5 characters + 1 for null terminator
            strncpy(strBuffer, (char *)valuePtr, settings[i].maxVal - 1);
            strBuffer[settings[i].maxVal - 1] = '\0'; // Null-terminate the string

            // Display the string directly
            sprintf(buffer, "%s: %s", name, strBuffer);
        }

        // Draw the setting string on the display
        if (hovered < i + 3)
        {
            u8g2.drawStr(listLeftSpacing, listYStart, buffer);
            if (hovered == i)
                u8g2.drawRFrame(listLeftSpacing - 4, listYStart - 10, u8g2.getStrWidth(buffer) + 8, 13, 5);
        }

        listYStart += rowSpacing; // Move down for the next setting
    }

    /*Change Setting Value*/
    if (rotaryEncoderSwitchValue == PRESSED && rotaryEncoderButtonReady)
    {
        // When a setting is selected, open a popup window to select a new value, then save the value
        if (settings[hovered].type == BOOLEAN)
        {
            String options[] = {"Off", "On"};
            bool newValue = openPopupMultiChoice(settings[hovered].name, options, 2, *(bool *)settings[hovered].value);

            *(bool *)settings[hovered].value = newValue;
            saveValues();

            rotaryEncoderButtonReady = false;
        }

        // When a setting is selected, open a popup window to select a new value, then save the value
        if (settings[hovered].type == INTEGER)
        {
            long min = settings[hovered].minVal;
            long max = settings[hovered].maxVal;
            long val = *(long int *)settings[hovered].value;

            long int newValue = openPopupNumber(settings[hovered].name, constrain(val, min, max), min, max);
            *(long int *)settings[hovered].value = newValue;
            saveValues();

            rotaryEncoderButtonReady = false;
        }

        if (settings[hovered].type == STRING)
        {
            String newValue = openPopupString(settings[hovered].name, String((char *)settings[hovered].value), settings[hovered].maxVal);
            strncpy((char *)settings[hovered].value, newValue.c_str(), settings[hovered].maxVal);
            saveValues();
            rotaryEncoderButtonReady = false;
        }
    }

    /*Draw Scroll Bar*/
    if (numSettings > 1)
    {
        drawScrollBar(numSettings, hovered);
    }
}