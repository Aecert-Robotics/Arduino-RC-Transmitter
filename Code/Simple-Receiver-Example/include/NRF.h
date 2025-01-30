#pragma once

#include <Arduino.h>
#include "RF24.h" 
#include "nRF24L01.h" 
#include <vectors.h>

extern RF24 radio;

extern unsigned long rc_send_interval;

// 32 Byte max
struct RC_Data_Package {
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
    byte reserved : 2;  // 2 bits padding

};

struct Ack_Data_Package {
    byte connected:1; // 1 bit
    byte reserved:7;  // 7 bits padding
};

extern RC_Data_Package rc_data;
extern Ack_Data_Package ack_Data;

void setupNRF();
void receiveNRFData();
void RC_DisplayData();