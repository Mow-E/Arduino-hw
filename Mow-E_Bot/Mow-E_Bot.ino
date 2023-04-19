
#include "MeAuriga.h"

#define red 255, 0, 0
#define green 0, 255, 0
#define blue 0, 0, 255
#define white 255, 255, 255
#define black 0, 0, 0


MeUltrasonicSensor ultraSensor(PORT_10);
MeEncoderOnBoard motor1(SLOT1);
MeEncoderOnBoard motor2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);

const int BUFFER_SIZE = 128;
const int SPEED = 100;
const int STOP = 0;



char incomingMessage[BUFFER_SIZE] = { 0 };

bool isForward = false;
bool isBackward = false;
bool isRotate = false;
bool isStop = true;

void setup() {
  Serial.begin(9600);
  rgbLED.setpin(44);
  //rgbLED.setColorAt(11, 0,0,0);
}

void loop() {
  handleSerialCommunication();
  motorController();
  setLEDLoop(0, 0, 0);
}

void handleSerialCommunication() {
  while (Serial.available() > 0) {
    Serial.readBytesUntil('\n', incomingMessage, BUFFER_SIZE);
    handleReceivedMessage();
  }
}
void handleReceivedMessage() {
  if (incomingMessage[0] == 'F') {
    isForward = true;
    isBackward = false;
    isRotate = false;
    isStop = false;
  }
  if (incomingMessage[0] == 'S') {
    isForward = false;
    isBackward = false;
    isRotate = false;
    isStop = true;
  }
  if (incomingMessage[0] == 'B') {
    isForward = false;
    isBackward = true;
    isRotate = false;
    isStop = true;
  }
  if (incomingMessage[0] == 'U') {
    Serial.print(getUltrasonicDistance());
  }
  if (incomingMessage[0] == 'E') {
    Serial.print(getlightSensor());
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'R') {
    setLEDLoop(red);
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'G') {
    setLEDLoop(green);
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'B') {
    setLEDLoop(blue);
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'W') {
    setLEDLoop(white);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'R') {
    setLEDLoopBlink(red);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'G') {
    setLEDLoopBlink(green);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'B') {
    setLEDLoopBlink(blue);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'W') {
    setLEDLoopBlink(white);
  }
}

void motorController() {
  if (isForward) {
    motor1.setMotorPwm(SPEED);
    motor2.setMotorPwm(-SPEED);
  }
  if (isBackward) {
    motor1.setMotorPwm(-SPEED);
    motor2.setMotorPwm(SPEED);
  }
  if (isStop) {
    motor1.setMotorPwm(STOP);
    motor2.setMotorPwm(STOP);
  }
  if (isRotate) {
  }
}
int getlightSensor() {
  return lineFollower.readSensors();
}

double getUltrasonicDistance() {
  return ultraSensor.distanceCm();
}

void setLEDLoop(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i <= 12; i++) {
    rgbLED.setColorAt(i, r, g, b);
    rgbLED.show();
  }
}

void setLEDLoopBlink(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 3; i++) {
    setLEDLoop(r, g, b);
    delay(1000);
    setLEDLoop(black);
    delay(1000);
  }
}
