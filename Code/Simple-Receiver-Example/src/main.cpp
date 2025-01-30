#include <Servo.h>
#include "vectors.h"
#include "Arduino.h"
#include "NRF.h"

#define COXA_PIN 3
#define FEMUR_PIN 5
#define TIBIA_PIN 6

Servo coxaServo;
Servo femurServo;
Servo tibiaServo;
float a1 = 28.1;  // coxa length
float a2 = 67.8;  // femur length
float a3 = 117.5; // tibia length
float legLength = a1 + a2 + a3;
Vector3 calibrationPosition = Vector3(0, a1 + a3, a2);
Vector3 targetPosition = Vector3(0, 0, 0);
Vector3 currentPosition = Vector3(0, 0, 0);

float offsets[3] = {0, 50, 0};

void JoystickMove();
void moveToPos(Vector3 pos);
float floatMap(float x, float in_min, float in_max, float out_min, float out_max);


void setup()
{
  Serial.begin(9600);
  coxaServo.attach(COXA_PIN);
  femurServo.attach(FEMUR_PIN);
  tibiaServo.attach(TIBIA_PIN);

  Vector3 pos1 = Vector3(0, 0, 0);
  Vector3 pos2 = Vector3(0, a1 + a3, a2);
 
  setupNRF();

  delay(100);
}

void loop()
{
  if (!rc_data.toggle_A) moveToPos(calibrationPosition);
  else JoystickMove();

  receiveNRFData();
}

void JoystickMove()
{
  int xVal = floatMap(rc_data.joyLeft_X, 0, 256, 130, -130);
  int yVal = floatMap(rc_data.joyRight_Y, 0, 256, 150, 70);
  int zVal = floatMap(rc_data.joyLeft_Y, 0, 256, -110, 40);

  targetPosition = Vector3(xVal, yVal, zVal);
  currentPosition = currentPosition + (targetPosition - currentPosition) * 0.02;

  moveToPos(currentPosition);
}

void moveToPos(Vector3 pos)
{

  float dis = Vector3(0, 0, 0).distanceTo(pos);
  if (dis > legLength)
  {
    Serial.println("Position out of reach");
    return;
  }

  float x = pos.x;
  float y = pos.y;
  float z = pos.z;

  float o1 = offsets[0];
  float o2 = offsets[1];
  float o3 = offsets[2];

  float theta1 = atan2(y, x) * (180 / PI) + o1; // base angle
  float l = sqrt(x * x + y * y);                
  float l1 = l - a1;
  float h = sqrt(l1 * l1 + z * z);

  float phi1 = acos(constrain((pow(h, 2) + pow(a2, 2) - pow(a3, 2)) / (2 * h * a2), -1, 1));
  float phi2 = atan2(z, l1);
  float theta2 = (phi1 + phi2) * 180 / PI + o2;
  float phi3 = acos(constrain((pow(a2, 2) + pow(a3, 2) - pow(h, 2)) / (2 * a2 * a3), -1, 1));
  float theta3 = (phi3 * 180 / PI) + o3;

  coxaServo.write(180 - theta1);
  femurServo.write(180 - theta2);
  tibiaServo.write(theta3);
  // Serial.println("Theta1: " + String(theta1) + " Theta2: " + String(theta2) + " Theta3: " + String(theta3));
  return;
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}