#include "NRF.h"
#include "Globals.h"

// Initialize the radio and addresses
RF24 radio(49, 4); // CE, CSN
bool radioNumber = 0;

unsigned long rc_send_interval = 50;

// Initialize the data packages
RC_Control_Data_Package rc_control_data;
RC_Settings_Data_Package rc_settings_data;
Hexapod_Settings_Data_Package hex_settings_data;
Hexapod_Sensor_Data_Package hex_sensor_data;

void setupNRF()
{
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.setPayloadSize(32);
    radio.setChannel(124);
    radio.enableAckPayload();
    radio.setRetries(5, 5);
    radio.openWritingPipe(nrfAddress);

    rc_control_data.type = RC_CONTROL_DATA;

    rc_control_data.joyLeft_X = 127;
    rc_control_data.joyLeft_Y = 127;
    rc_control_data.joyRight_X = 127;
    rc_control_data.joyRight_Y = 127;
    rc_control_data.potLeft = 50;
    rc_control_data.potRight = 50;
    rc_control_data.button_A = UNPRESSED;
    rc_control_data.button_B = UNPRESSED;
    rc_control_data.button_C = UNPRESSED;
    rc_control_data.button_D = UNPRESSED;
    rc_control_data.toggle_A = OFF;
    rc_control_data.toggle_B = OFF;
    rc_control_data.toggle_C = OFF;
    rc_control_data.toggle_D = OFF;
    rc_control_data.bumper_A = UNPRESSED;
    rc_control_data.bumper_B = UNPRESSED;
    rc_control_data.bumper_C = UNPRESSED;
    rc_control_data.bumper_D = UNPRESSED;
    rc_control_data.joyLeft_Button = UNPRESSED;
    rc_control_data.joyRight_Button = UNPRESSED;
    rc_control_data.gait = 0;

    rc_settings_data.type = RC_SETTINGS_DATA;
    // settings package
    rc_settings_data.type = RC_SETTINGS_DATA;
    rc_settings_data.calibrating = 0;
    rc_settings_data.increaseValue = UNPRESSED;
    rc_settings_data.decreaseValue = UNPRESSED;
    rc_settings_data.calibrationIndex = -1; // -1 means no calibration index is set
}

void sendNRFData(PackageType type)
{
    every(rc_send_interval)
    {
        bool report = false;

        if (type == RC_CONTROL_DATA)
        {
            report = radio.write(&rc_control_data, sizeof(rc_control_data)); // Send control data
        }
        else if (type == RC_SETTINGS_DATA)
        {
            report = radio.write(&rc_settings_data, sizeof(rc_settings_data)); // Send settings data
        }

        if (report)
        {
            if (radio.isAckPayloadAvailable())
            {
                byte ackType;
                radio.read(&ackType, sizeof(ackType));

                if (ackType == HEXAPOD_SETTINGS_DATA)
                {
                    radio.read(&hex_settings_data, sizeof(hex_settings_data));
                }
                else if (ackType == HEXAPOD_SENSOR_DATA)
                {
                    radio.read(&hex_sensor_data, sizeof(hex_sensor_data));

                    for (int i = 0; i < 6; i++)
                    {
                        foot_positions[i].x = hex_sensor_data.xPositions[i];
                        foot_positions[i].y = hex_sensor_data.yPositions[i];
                    }
                    /*
                    for (int i = 0; i < 6; i++)
                    {
                        Serial.print("Foot position ");
                        Serial.print(i);
                        Serial.print(": ");
                        Serial.print(hex_sensor_data.xPositions[i]);
                        Serial.print(", ");
                        Serial.println(hex_sensor_data.yPositions[i]);
                    }
                    */
                }

                // no data is being received
                else
                {
                    current_sensor_value = 0;
                    for (int i = 0; i < 6; i++)
                    {
                        foot_positions[i] = Vector2int(0, 0);
                    }
                }
            }
        }
    }
}