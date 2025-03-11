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
  Serial.print("Begin Setup");
  setupScreen();
  Serial.print("Screen Setup");
  setupInputs();
  Serial.print("Inputs Setup");
  Wire.begin();
  Serial.print("Wire Setup");
  loadValues();
  Serial.print("Values Loaded");
  setupNRF();
  Serial.print("NRF Setup");
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
