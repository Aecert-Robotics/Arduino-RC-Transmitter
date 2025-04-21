#include <Arduino.h>
#include "Page.h"
#include "Screen.h"
#include "Inputs.h"
#include "Popup.h"
#include "Globals.h"
#include "NRF.h"

float current_sensor_value = 0;
Vector2int foot_positions[6] = {Vector2int(0,0), Vector2int(0,0), Vector2int(0,0), Vector2int(0,0), Vector2int(0,0), Vector2int(0,0)};
int legsXMultiplier[6] = {-1, -1, -1, 1, 1, 1};
int legsYMultiplier[6] = {-1, -1, -1, 1, 1, 1};
int rotationMultiplier[6] = {1, 0, -1, 1, 0 , -1};

void rotateLeg(int angle, Vector3 &leg, Vector2 pivot) {
    // Translate line so pivot point is at the origin
    if(angle == 0) return;

    leg.x -= pivot.x;
    leg.y -= pivot.y;
    float angleRad = radians(angle);

    // Rotate point by angle
    int x_rotated = leg.x * cos(angleRad) - leg.y * sin(angleRad);
    int y_rotated = leg.x * sin(angleRad) + leg.y * cos(angleRad);

    // Translate point back to original position
    leg.x = x_rotated + pivot.x;
    leg.y = y_rotated + pivot.y;
}

void HomePage::init()
{
    rotaryEncoderButtonReady = false;
    rotaryEncoderSwitchValue = UNPRESSED;
}

void HomePage::loop()
{    
    rotaryEncoderSwitchValue = getRotaryEncoderSwitchValue();

    rc_control_data.joyLeft_X = getJoyValue(A).x;
    rc_control_data.joyLeft_Y = getJoyValue(A).y;
    rc_control_data.joyLeft_Button = getJoyButtonValue(A);
    rc_control_data.joyRight_X = getJoyValue(B).x;
    rc_control_data.joyRight_Y = getJoyValue(B).y;
    rc_control_data.joyRight_Button = getJoyButtonValue(B);
    rc_control_data.potLeft = getPotValue(A);
    rc_control_data.potRight = getPotValue(B);
    rc_control_data.button_A = getButtonValue(A);
    rc_control_data.button_B = getButtonValue(B);
    rc_control_data.button_C = getButtonValue(C);
    rc_control_data.button_D = getButtonValue(D);
    rc_control_data.toggle_A = getSwitchValue(A);
    rc_control_data.toggle_B = getSwitchValue(B);
    rc_control_data.toggle_C = getSwitchValue(C);
    rc_control_data.toggle_D = getSwitchValue(D);
    rc_control_data.bumper_A = getBumperValue(A);
    rc_control_data.bumper_B = getBumperValue(B);
    rc_control_data.bumper_C = getBumperValue(C);
    rc_control_data.bumper_D = getBumperValue(D);   
    rc_control_data.gyro_X = readGyro().x;
    rc_control_data.gyro_Y = readGyro().y;    
    rc_control_data.gait = selectedGait;

    //if bumper A is pressed, do not draw the screen
    if(rc_control_data.bumper_A == PRESSED) return;

    startTime = millis(); 
    
         

    int offset = 35;
    int textY = 6;
    int textX = 1;

    int lineY = 7;
    int lineL = 19;

    /*Switch Labels*/
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(textX, textY, "Gyro");
    if (getSwitchValue(A) == ON)
        u8g2.drawHLine(textX, lineY, lineL);

    u8g2.drawStr(textX + offset, textY, "Step");
    if (getSwitchValue(B) == ON)
        u8g2.drawHLine(textX + offset, lineY, lineL);

    u8g2.drawStr(textX + offset * 2, textY, "Easy");
    if (getSwitchValue(C) == ON)
        u8g2.drawHLine(textX + offset * 2, lineY, lineL);

    /*
    u8g2.drawStr(textX + offset * 3, textY, "4th Toggle");
    if (getSwitchValue(D) == ON)
        u8g2.drawHLine(textX + offset * 3, lineY, lineL);
    */
    

    /*Main Menu Button*/
    drawStringButton(4, 59, "E", "Menu", FONT_TEXT);


    

    if (rotaryEncoderSwitchValue == UNPRESSED && !rotaryEncoderButtonReady) rotaryEncoderButtonReady = true;    
    if (rotaryEncoderSwitchValue == PRESSED && rotaryEncoderButtonReady) currentPage = mainMenuPage;
    

    /*Gait Display*/
    drawStringButton(4, 25, "B", "Gait", FONT_TEXT);
    u8g2.setFont(FONT_BOLD_HEADER);
    u8g2.setFontMode(1);
    u8g2.drawStr(1, 43, gaitStrings[selectedGait].c_str());


    /*Info Display*/
    offset = 13;
    textX = 109;
    textY = 23;
    int iconX = 97;

    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(textX, textY, "100%");
    u8g2.drawStr(textX, textY + offset, String(current_sensor_value).c_str());
    u8g2.drawStr(textX, textY + offset * 2, (String(getPotValue(A)) + "%").c_str());
    u8g2.drawStr(textX, textY + offset * 3, (String(getPotValue(B)) + "%").c_str());

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(iconX + 1, textY, 0xe15e);
    u8g2.drawGlyph(iconX, textY + offset, 0xe09e);
    u8g2.drawGlyph(iconX, textY + offset * 2, 0xe206);
    u8g2.drawGlyph(iconX, textY + offset * 3, 0xe10f);


    // For the legs:
    // x,y = leg coordinates
    // z = is leg touching ground (0 is raised, 1 is on ground)


    for (int i = 0; i < 6; i++) {
        legs[i].x = foot_positions[i].y/13;
        legs[i].y = foot_positions[i].x/13;
        legs[i].z = 0;
        rotateLeg(55*rotationMultiplier[i], legs[i], Vector2(2,0));
        legs[i].x *= legsXMultiplier[i];
        legs[i].y *= legsYMultiplier[i];
    }
    
    

    drawHexapod(Vector2(64, 37), legs[0], legs[1], legs[2], legs[3], legs[4], legs[5]);
    u8g2.drawRFrame(38, 12, 52, 52, 5);
    

    totalDrawTime = millis() - startTime;

    //u8g2.setFont(FONT_TEXT_MONOSPACE);
    //u8g2.drawStr(61, 60, String(getRotaryEncoderTotalSpins()).c_str());


    /*Gait Selection Popup*/
    if(getButtonValue(B) == PRESSED){        
        selectedGait = Gaits(openPopupMultiChoice("Select a Gait", gaitStrings, gaitCount, selectedGait));
        rotaryEncoderButtonReady = false;
    }   
    
}