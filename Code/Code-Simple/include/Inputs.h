#pragma once

#include <Arduino.h>
#include "Helpers.h"
#include <Wire.h>
#include <MPU6050_light.h>

// Pin definitions
#define ButtonA_Pin 12
#define ButtonB_Pin 10
#define ButtonC_Pin 8
#define ButtonD_Pin 6

#define BumperA_Pin 27 // Left front
#define BumperB_Pin 29 // Left back
#define BumperC_Pin 38 // Right front
#define BumperD_Pin 37 // Right back

#define SwitchA_Pin 35
#define SwitchB_Pin 34
#define SwitchC_Pin 33
#define SwitchD_Pin 32

#define JoyLeftButton_Pin 31
#define JoyRightButton_Pin 36

#define PotA_Pin A0
#define PotB_Pin A14

#define BatteryVoltage_Pin A8

#define RotaryEncoderButton_Pin 39

// External variable declarations
extern Vector3 gad;
extern MPU6050 mpu;
extern unsigned long timeSinceLastInput; // Add this line
extern String lastInputName; // Add this line

// Function declarations
int getPotValue(IOLabels label);
String getPotsString();
int getButtonValue(IOLabels label);
String getButtonsString();
int getBumperValue(IOLabels label);
String getBumpersString();
int getSwitchValue(IOLabels label);
String getSwitchesString();
int getJoyButtonValue(IOLabels label);
String getJoyButtonsString();
Vector2 getJoyValue(IOLabels label);
String getJoyValueString(IOLabels label);
Vector3 readGyro();
String getGyroString();
void isrA();
void isrB();
int getRotaryEncoderSpins();
int getRotaryEncoderTotalSpins();
int getRotaryEncoderSwitchValue();
float getBatteryVoltageValue();

String getRotaryEncoderSwitchString();
void setupInputs();

unsigned long getTimeSinceButtonPressed(IOLabels label);
unsigned long getTimeSinceBumperPressed(IOLabels label);
unsigned long getTimeSinceSwitchChanged(IOLabels label);
unsigned long getTimeSinceJoyButtonPressed(IOLabels label);
unsigned long getTimeSincePotChanged(IOLabels label);
unsigned long getTimeSinceLastInput(); // Add this line
String getLastInputName(); // Add this line

