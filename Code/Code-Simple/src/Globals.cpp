#include <Arduino.h>
#include "Globals.h"
#include "Helpers.h"
#include "Page.h"
#include <EEPROM.h>
#include "NRF.h"

HomePage *homePage = new HomePage();
MainMenuPage *mainMenuPage = new MainMenuPage();
ControlsPage *controlsPage = new ControlsPage();
SettingsPage *settingsPage = new SettingsPage();

Page *previousPage = nullptr;
Page *currentPage = homePage;

void loadValues()
{
    // Load NRF Address
    for (int i = 0; i < EEPROM_NRF_ADDRESS_ARRAY_SIZE; i++) {
        nrfAddress[i] = EEPROM.read(EEPROM_NRF_ADDRESS_ADDR + i);
    }
}

void saveValues(){
    // Save NRF Address (array of bytes)
    bool addressChanged = false;
    for (int i = 0; i < EEPROM_NRF_ADDRESS_ARRAY_SIZE; i++) {
        if (EEPROM.read(EEPROM_NRF_ADDRESS_ADDR + i) != nrfAddress[i]) {
            addressChanged = true;
            break;
        }
    }
    if (addressChanged) {
        for (int i = 0; i < EEPROM_NRF_ADDRESS_ARRAY_SIZE; i++) {
            EEPROM.update(EEPROM_NRF_ADDRESS_ADDR + i, nrfAddress[i]);
        }
        setupNRF();
    }
}
