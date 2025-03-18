#include <Arduino.h>
#include <NRF.h>

uint8_t address[6] = "HEX01";
RC_Data_Package rc_data;
Ack_Data_Package ack_data;

#define CE_PIN 26
#define CSN_PIN 27
#define INTERVAL_MS_SIGNAL_LOST 1000
#define INTERVAL_MS_SIGNAL_RETRY 250
RF24 radio(CE_PIN, CSN_PIN);

unsigned long lastSignalMillis = 0;
void setupNRF()
{
    ack_data.connected = 1;
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, address);
    radio.enableAckPayload();
    radio.startListening();
}
void receiveNRFData()
{
    radio.writeAckPayload(1, &ack_data, sizeof(ack_data));

    unsigned long currentMillis = millis();
    if (radio.available() > 0)
    {
        radio.read(&rc_data, sizeof(rc_data));
        lastSignalMillis = currentMillis;
    }

    // RC_DisplayData();

    if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST)
    {
        Serial.println("We have lost connection, preventing unwanted behavior");
        delay(INTERVAL_MS_SIGNAL_RETRY);
    }
}

void RC_DisplayData()
{
    Serial.println("JLX: " + String(rc_data.joyLeft_X) +
                   "  JLY: " + String(rc_data.joyLeft_Y) +
                   "  JRX: " + String(rc_data.joyRight_X) +
                   "  JRY: " + String(rc_data.joyRight_Y) +
                   "  JLB: " + String(rc_data.joyLeft_Button) +
                   "  JRB: " + String(rc_data.joyRight_Button) +
                   "  PL: " + String(rc_data.potLeft) +
                   "  PR: " + String(rc_data.potRight) +
                   "  BA: " + String(rc_data.button_A) +
                   "  BB: " + String(rc_data.button_B) +
                   "  BC: " + String(rc_data.button_C) +
                   "  BD: " + String(rc_data.button_D) +
                   "  TA: " + String(rc_data.toggle_A) +
                   "  TB: " + String(rc_data.toggle_B) +
                   "  TC: " + String(rc_data.toggle_C) +
                   "  TD: " + String(rc_data.toggle_D) +
                   "  BA: " + String(rc_data.bumper_A) +
                   "  BB: " + String(rc_data.bumper_B) +
                   "  BC: " + String(rc_data.bumper_C) +
                   "  BD: " + String(rc_data.bumper_D));
}