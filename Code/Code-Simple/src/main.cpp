#include <Arduino.h>

#include "Wire.h"
#include "Helpers.h"
#include "Globals.h"
#include "Inputs.h"
#include "NRF.h"
#include "Screen.h"
#include "Page.h"

const bool DEBUG_PRINT = false;
#define UpdateScreenInterval 100
unsigned long drawTimerStart = 0;
unsigned long drawTimerEnd = 0;
bool drewScreen = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Begin Initialization");
  Serial.println("Initializing Screen");
  setupScreen();

  Serial.println("Initializing Wire Library");
  Wire.begin();

  Serial.println("Initializing Inputs");
  setupInputs(); 
  
  Serial.println("Loading Saved Values from EEPROM");
  loadValues();
  
  Serial.println("Initializing NRF");
  setupNRF();
  
  Serial.println("Initialization Finished");
}

void loop()
{
  drewScreen = false;
  every(UpdateScreenInterval)
  {  

    if (previousPage != currentPage)
    {
      currentPage->init();
      previousPage = currentPage;
    }

    u8g2.clearBuffer();
    currentPage->loop();
    u8g2.sendBuffer();

    drewScreen = true;
  }

  if (!drewScreen)
  {
    mpu.update();
    sendNRFData();
  }
}
