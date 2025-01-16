#include <Arduino.h>
#include "Page.h"
#include "Screen.h"
#include "Inputs.h"
#include "Globals.h"
#include "NRF.h"

void MainMenuPage::init()
{
    backButtonReady = false;
    rotaryEncoderButtonReady = false;
    rotaryEncoderSwitchValue = UNPRESSED;

    //ignore spins when entering the page
    getRotaryEncoderSpins();
}

void MainMenuPage::loop()
{   
    rotaryEncoderSwitchValue = getRotaryEncoderSwitchValue();

    if (getButtonValue(A) == UNPRESSED) backButtonReady = true;
    if (getButtonValue(A) == PRESSED  && backButtonReady) currentPage = homePage;
    
    drawPageHeader("< Home < ", "Menu");
    
    /*Debug Text*/
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(115, 63, String(getRotaryEncoderTotalSpins()).c_str());

    u8g2.setFont(FONT_TEXT_MONOSPACE);

    int rowSpacing = 17;
    int y = 25;

    int increment = 0;
    int spins = getRotaryEncoderSpins();
    if (spins > 0) increment = 1;
    if (spins < 0) increment = -1;

    hovered += increment;
    if (hovered >= numChoices) hovered = numChoices - 1;
    else if (hovered < 0) hovered = 0;


    //Controls Page
    static unsigned char controls_20icon_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0xbe, 0x07, 0xfb, 0x0f,
        0xf1, 0x09, 0xfb, 0x09, 0x0f, 0x0f, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00};
    u8g2.drawXBM(4, y - 9, 12, 12, controls_20icon_bits);
    u8g2.drawStr(20, y, "Controls");
    if (hovered == 0)
        u8g2.drawRFrame(1, y - 11, 61, 16, 5);


    //Settings Page
    u8g2.setFont(u8g2_font_twelvedings_t_all);
    u8g2.drawGlyph(5, y + 2 + rowSpacing, 0x0047);
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(20, y + rowSpacing, "Settings");
    if (hovered == 1)
        u8g2.drawRFrame(1, y + rowSpacing - 11, 61, 16, 5);


    if (rotaryEncoderSwitchValue == UNPRESSED && !rotaryEncoderButtonReady) rotaryEncoderButtonReady = true;    
    if (rotaryEncoderSwitchValue == PRESSED && rotaryEncoderButtonReady)
    {
        switch (hovered)
        {
        case 0:
            currentPage = controlsPage;
            return;

        case 1:
            currentPage = settingsPage;
            return;

        default:
            return;
        }
    }
}