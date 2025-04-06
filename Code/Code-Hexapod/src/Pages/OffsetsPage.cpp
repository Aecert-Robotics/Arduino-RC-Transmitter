#include <Arduino.h>
#include "Page.h"
#include "Screen.h"
#include "Inputs.h"
#include "Globals.h"
#include "NRF.h"

int hovered = 0;

Vector2 offsetLocations[OFFSETS_COUNT] = {
    Vector2(66,30), 
    Vector2(52,25),
    Vector2(38,20),

    Vector2(62,41),
    Vector2(48,41),
    Vector2(34,41),

    Vector2(66,52),
    Vector2(52,57),
    Vector2(38,62),

    Vector2(88,52),
    Vector2(102,57),
    Vector2(116,62),

    Vector2(92,41),
    Vector2(106,41),
    Vector2(120,41),

    Vector2(88,30),
    Vector2(102,25),
    Vector2(116,20)    
    };

unsigned long lastIncrementTime = 0;
unsigned long maxScrollDelay = 200; // Start with a 200ms delay
unsigned long currentScrollDelay = maxScrollDelay;
unsigned long minScrollDelay = 40;

unsigned long lastCursorBlinkTime = 0;
const unsigned long cursorBlinkInterval = 500; // 500ms interval for cursor blink
bool cursorVisible = true;

void OffsetsPage::init()
{
    lastCursorBlinkTime = millis(); // Initialize the cursor blink timer

    rotaryEncoderButtonReady = false;
    rotaryEncoderSwitchValue = UNPRESSED;
    rc_settings_data.calibrating = 1;
}

void OffsetsPage::loop()
{       
    rotaryEncoderSwitchValue = getRotaryEncoderSwitchValue();

    // Exit the page
    if (getButtonValue(A) == PRESSED) 
    {
        currentPage = mainMenuPage;
        rc_settings_data.calibrating = 0;
    }
    drawPageHeader("< Home < Menu < ", "Offsets");

    /*Debug Text
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(115, 63, String(getRotaryEncoderTotalSpins()).c_str());*/

    drawHexapodBody(Vector2(76, 38));

    int buttonSpacing = 14;
    drawStringButton(5, 17 + buttonSpacing * 2, "C", "+1", FONT_TEXT_MONOSPACE);
    drawStringButton(5, 17 + buttonSpacing * 3, "D", "-1", FONT_TEXT_MONOSPACE);

    /*Scrolling*/
    int increment = 0;
    int spins = getRotaryEncoderSpins();

    int previousHovered = hovered; // Store the previous hovered value

    // limit to scrolling 1 item per frame
    if (spins > 0)
        increment = 1;
    if (spins < 0)
        increment = -1;

    // stop scrolling at ends of list
    hovered += increment;
    if (hovered > OFFSETS_COUNT-1)
        hovered = 0;
    else if (hovered < 0)
        hovered = OFFSETS_COUNT-1;

    //Tell the hexapod which servo to calibrate
    rc_settings_data.calibrationIndex = hovered; 

    // Reset cursor blink timer if hovered has changed
    if (hovered != previousHovered) {
        lastCursorBlinkTime = millis();
        cursorVisible = true;
    }

    // Increment or decrement offsets[hovered] based on button presses
    rc_settings_data.increaseValue = UNPRESSED;
    rc_settings_data.decreaseValue = UNPRESSED;

    unsigned long currentTime = millis();
    if (currentTime - lastIncrementTime >= currentScrollDelay || currentScrollDelay == maxScrollDelay)
    {     
        //increment button pressed
        if (getButtonValue(C) == PRESSED)
        {
            //tell the hexapod to increase the offset of the servo thats being calibrated
            if(hex_settings_data.calibrationOffsets[hovered] < 99) rc_settings_data.increaseValue = PRESSED;

            lastIncrementTime = currentTime;
            currentScrollDelay = max(minScrollDelay, currentScrollDelay - minScrollDelay); // Decrease delay
        }

        //decrement button pressed
        else if (getButtonValue(D) == PRESSED)
        {
            //tell the hexapod to decrease the offset of the servo thats being calibrated
            if(hex_settings_data.calibrationOffsets[hovered] > -99) rc_settings_data.decreaseValue = PRESSED;

            lastIncrementTime = currentTime;
            currentScrollDelay = max(minScrollDelay, currentScrollDelay - minScrollDelay); // Decrease delay
        }
        else
        {
            currentScrollDelay = maxScrollDelay; // Reset delay when no button is pressed            
        }
    }

    // Handle cursor blinking
    if (currentTime - lastCursorBlinkTime >= cursorBlinkInterval)
    {
        cursorVisible = !cursorVisible;
        lastCursorBlinkTime = currentTime;
    }

    String joint = "Coxa";
    if (hovered % 3 == 1)
        joint = "Femur";
    if (hovered % 3 == 2)
        joint = "Tibia";

    String legNumber = "Leg " + String(hovered / 3 + 1);

    u8g2.setFont(FONT_HEADER);
    u8g2.drawStr(1, 23, legNumber.c_str());
    u8g2.setFont(FONT_TEXT);
    u8g2.drawStr(1, 34, joint.c_str());

    u8g2.setFont(FONT_TINY_NUMBERS);    
    for (int i = 0; i < OFFSETS_COUNT; i++)
    {
        String offsetStr = String(hex_settings_data.calibrationOffsets[i]);
        int strWidth = u8g2.getStrWidth(offsetStr.c_str());        
        int centeredX = offsetLocations[i].x - (strWidth / 2); // Center the text
        if (i == hovered && cursorVisible) {            
            // Draw a frame around the hovered value
            u8g2.drawRFrame(centeredX - 2, offsetLocations[i].y - 7, strWidth + 3, 9, 2);
        }
        u8g2.drawStr(centeredX, offsetLocations[i].y, offsetStr.c_str());
    }
}