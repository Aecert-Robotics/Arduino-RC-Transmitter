#include <Arduino.h>
#include <NRF.h>

uint8_t address[6] = "HEX01";
RC_Data_Package dataPackage;
Ack_Data_Package ackPackage;

#define CE_PIN 7
#define CSN_PIN 8
#define INTERVAL_MS_SIGNAL_LOST 1000
#define INTERVAL_MS_SIGNAL_RETRY 250
RF24 radio(CE_PIN, CSN_PIN);

unsigned long lastSignalMillis = 0;
void setupNRF()
{
    ackPackage.connected = 1;
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, address);
    radio.enableAckPayload();
    radio.startListening();
}
void recieveNRFData()
{
    radio.writeAckPayload(1, &ackPackage, sizeof(ackPackage));

    unsigned long currentMillis = millis();
    if (radio.available() > 0)
    {
        radio.read(&dataPackage, sizeof(dataPackage));
        lastSignalMillis = currentMillis;
    }
    // RC_DisplayData();
    if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST)
    {
        lostConnection();
    }
}
void lostConnection()
{
    Serial.println("We have lost connection, preventing unwanted behavior");
    delay(INTERVAL_MS_SIGNAL_RETRY);
}

void RC_DisplayData()
{
    Serial.println("JLX: " + String(dataPackage.joyLeft_X) +
                   "  JLY: " + String(dataPackage.joyLeft_Y) +
                   "  JRX: " + String(dataPackage.joyRight_X) +
                   "  JRY: " + String(dataPackage.joyRight_Y) +
                   "  JLB: " + String(dataPackage.joyLeft_Button) +
                   "  JRB: " + String(dataPackage.joyRight_Button) +
                   "  PL: " + String(dataPackage.potLeft) +
                   "  PR: " + String(dataPackage.potRight) +
                   "  BA: " + String(dataPackage.button_A) +
                   "  BB: " + String(dataPackage.button_B) +
                   "  BC: " + String(dataPackage.button_C) +
                   "  BD: " + String(dataPackage.button_D) +
                   "  TA: " + String(dataPackage.toggle_A) +
                   "  TB: " + String(dataPackage.toggle_B) +
                   "  TC: " + String(dataPackage.toggle_C) +
                   "  TD: " + String(dataPackage.toggle_D) +
                   "  BA: " + String(dataPackage.bumper_A) +
                   "  BB: " + String(dataPackage.bumper_B) +
                   "  BC: " + String(dataPackage.bumper_C) +
                   "  BD: " + String(dataPackage.bumper_D));
}