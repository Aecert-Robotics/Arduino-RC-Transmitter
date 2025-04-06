#pragma once

#include <Arduino.h>
#include "Helpers.h"
#include <SPI.h>
#include <RF24.h>

// Define the radio and addresses
extern RF24 radio;

extern unsigned long rc_send_interval;

// Define the data packages

// 10 Bytes Total
struct RC_Control_Data_Package {
    byte type; // 1 byte
    
    byte joyLeft_X; // 1 byte
    byte joyLeft_Y; // 1 byte
    
    byte joyRight_X; // 1 byte    
    byte joyRight_Y; // 1 byte  

    byte potLeft; // 1 byte
    byte potRight; // 1 byte 

    //1 byte
    byte button_A:1; // 1 bit
    byte button_B:1; // 1 bit
    byte button_C:1; // 1 bit
    byte button_D:1; // 1 bit
    byte toggle_A:1; // 1 bit
    byte toggle_B:1; // 1 bit
    byte toggle_C:1; // 1 bit
    byte toggle_D:1; // 1 bit

    //1 byte
    byte bumper_A:1; // 1 bit
    byte bumper_B:1; // 1 bit
    byte bumper_C:1; // 1 bit
    byte bumper_D:1; // 1 bit
    byte joyLeft_Button:1; // 1 bit
    byte joyRight_Button:1; // 1 bit
    byte reserved:2;  // 2 bits padding

    byte gait;  // 1 byte
};

// 20 Bytes Total
struct RC_Settings_Data_Package {
    byte type; // 1 byte
    
    byte calibrating:1; //1 bit
    byte increaseValue:1; //1 bit
    byte decreaseValue:1; //1 bit
    byte reserved:5;              //7 bits padding, 1 byte total

    int8_t calibrationIndex;   //1 byte

};

// 19 Bytes Total
struct Hexapod_Settings_Data_Package {
    byte type; // 1 byte
    int8_t calibrationOffsets[18]; // 18 bytes
};

// 29 Bytes Total
struct Hexapod_Sensor_Data_Package {
    byte type; // 1 byte
    int8_t xPositions[6]; // 6 bytes
    int8_t yPositions[6]; // 6 bytes
};

// Declare the data package variables
extern RC_Control_Data_Package rc_control_data;
extern RC_Settings_Data_Package rc_settings_data;
extern Hexapod_Settings_Data_Package hex_settings_data;
extern Hexapod_Sensor_Data_Package hex_sensor_data;

// Function declarations
void setupNRF();
void sendNRFData(PackageType type);