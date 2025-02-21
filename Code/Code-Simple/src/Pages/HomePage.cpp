#include <Arduino.h>
#include "Page.h"
#include "Screen.h"
#include "Inputs.h"
#include "Popup.h"
#include "Globals.h"
#include "NRF.h"

void HomePage::init()
{
    rotaryEncoderButtonReady = false;
    rotaryEncoderSwitchValue = UNPRESSED;
}

void HomePage::loop()
{
    mpu.update();
    /*Rotary Encoder Init*/
    rotaryEncoderSwitchValue = getRotaryEncoderSwitchValue();
    if (rotaryEncoderSwitchValue == UNPRESSED && !rotaryEncoderButtonReady)
        rotaryEncoderButtonReady = true;
    if (rotaryEncoderSwitchValue == PRESSED && rotaryEncoderButtonReady)
        currentPage = mainMenuPage;

    /*Input Reading*/
    rc_data.joyLeft_X = getJoyValue(A).x;
    rc_data.joyLeft_Y = getJoyValue(A).y;
    rc_data.joyLeft_Button = getJoyButtonValue(A);

    rc_data.joyRight_X = getJoyValue(B).x;
    rc_data.joyRight_Y = getJoyValue(B).y;
    rc_data.joyRight_Button = getJoyButtonValue(B);

    rc_data.potLeft = getPotValue(A);
    rc_data.potRight = getPotValue(B);
    rc_data.button_A = getButtonValue(A);
    rc_data.button_B = getButtonValue(B);
    rc_data.button_C = getButtonValue(C);
    rc_data.button_D = getButtonValue(D);

    rc_data.toggle_A = getSwitchValue(A);
    rc_data.toggle_B = getSwitchValue(B);
    rc_data.toggle_C = getSwitchValue(C);
    rc_data.toggle_D = getSwitchValue(D);

    rc_data.bumper_A = getBumperValue(A);
    rc_data.bumper_B = getBumperValue(B);
    rc_data.bumper_C = getBumperValue(C);
    rc_data.bumper_D = getBumperValue(D);
    startTime = millis();

    int textY = 6;
    int textX = 1;

    int lineY = 10;

    /*Rotary Encoder Label*/
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    u8g2.drawStr(40, 59, String(getRotaryEncoderTotalSpins()).c_str());

    /*Switch Labels*/
    u8g2.drawStr(textX, textY, getSwitchesString().c_str());

    /*Button Labels*/
    u8g2.drawStr(textX, textY + lineY, getButtonsString().c_str());

    /*Bumper Labels*/
    u8g2.drawStr(textX, textY + lineY * 2, getBumpersString().c_str());

    /*Pot Labels*/
    String potsString = getPotsString();
    int splitIndex = potsString.indexOf("PotB:");
    String potAString = potsString.substring(0, splitIndex);
    String potBString = potsString.substring(splitIndex);

    u8g2.drawStr(textX, textY + lineY * 3, potAString.c_str());
    u8g2.drawStr(textX, textY + lineY * 4, potBString.c_str());

    /*Main Menu Button*/
    drawStringButton(4, 59, "E", "Menu", FONT_TEXT);

    /* Battery Voltage */
    float voltage = getBatteryVoltageValue();
    float percentage = mapFloat(voltage, 6, 8, 0, 100);
    u8g2.drawStr(110, 10, (String(percentage) + "%").c_str());

    /* WiFi Symbol */
    u8g2.setFont(u8g2_font_siji_t_6x10);
    if (ack_data.connected)
    {
        // Draw WiFi symbol
        u8g2.drawGlyph(110, 20, 0xe21a);
    }
    else
    {
        // Draw crossed out WiFi symbol
        u8g2.drawGlyph(110, 20, 0xe217);
    }

    /*Joystick and Gyro Circles*/
    int circleRadius = 12;
    int circleCenterX = 90;
    int circleCenterY = 40;
    int pixelRadius = 8;
    int gyroRadius = 14;
    u8g2.setFont(FONT_TEXT_MONOSPACE);
    mpu.update();
    // Left Joystick
    u8g2.drawCircle(circleCenterX - 20, circleCenterY, circleRadius);
    Vector2 joyLeft = getJoyValue(A);
    int joyLeftX = circleCenterX - 20 + map(joyLeft.x, 0, 254, -pixelRadius, pixelRadius);
    int joyLeftY = circleCenterY - map(joyLeft.y, 0, 254, -pixelRadius, pixelRadius);
    u8g2.drawBox(joyLeftX - 1, joyLeftY - 1, 2, 2);                               // Draw 2x2 pixel
    u8g2.drawStr(circleCenterX - 30, circleCenterY + circleRadius + 10, "Joy L"); // Label

    // Right Joystick
    u8g2.drawCircle(circleCenterX + 20, circleCenterY, circleRadius);
    Vector2 joyRight = getJoyValue(B);
    int joyRightX = circleCenterX + 20 - map(joyRight.x, 0, 254, -pixelRadius, pixelRadius); // Flipped X-axis
    int joyRightY = circleCenterY - map(joyRight.y, 0, 254, -pixelRadius, pixelRadius);
    u8g2.drawBox(joyRightX - 1, joyRightY - 1, 2, 2);                             // Draw 2x2 pixel
    u8g2.drawStr(circleCenterX + 10, circleCenterY + circleRadius + 10, "Joy R"); // Label
    mpu.update();
    // Gyro
    u8g2.drawCircle(circleCenterX, circleCenterY - 18, circleRadius);
    Vector3 gyro = readGyro();
    int gyroX = circleCenterX + map(-gyro.x, -90, 90, -gyroRadius, gyroRadius);      // Negate X
    int gyroY = circleCenterY - 18 + map(-gyro.y, -90, 90, -gyroRadius, gyroRadius); // Negate Y
    u8g2.drawBox(gyroX - 1, gyroY - 1, 2, 2);                                        // Draw 2x2 pixel
    u8g2.drawStr(circleCenterX - 10, circleCenterY - 18 - circleRadius - 3, "Gyro"); // Label

    totalDrawTime = millis() - startTime;
}