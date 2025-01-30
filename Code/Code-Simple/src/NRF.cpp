#include "NRF.h"
#include "Globals.h"

unsigned long rc_send_interval = 50;
RC_Data_Package rc_data;
Ack_Data_Package ack_data;

#define CE_PIN 49
#define CSN_PIN 4
#define INTERVAL_MS_TRANSMISSION 250
RF24 radio(CE_PIN, CSN_PIN);
// NRF24L01 buffer limit is 32 bytes (max struct size)

void setupNRF()
{
    ack_data.connected = 0;
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload();
    radio.setRetries(5, 5);
    radio.openWritingPipe(nrfAddress);
}
void sendNRFData()
{
    radio.write(&rc_data, sizeof(rc_data)); // Send Data Package

    if (radio.isAckPayloadAvailable())
    {
        //Serial.println("Ack Payload Available");
        radio.read(&ack_data, sizeof(ack_data));
    }
    else{
        ack_data.connected = 0;
    }
}
