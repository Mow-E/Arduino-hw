
#include "MeAuriga.h"

MeUltrasonicSensor ultraSensor(PORT_10);
MeEncoderOnBoard motor1(SLOT1);
MeEncoderOnBoard motor2(SLOT2);

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
}

void loop() {
  handleSerialCommunication();
  motorController();
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

double getUltrasonicDistance() {
  return ultraSensor.distanceCm();
}
