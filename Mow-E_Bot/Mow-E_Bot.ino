
#include "MeAuriga.h"

MeUltrasonicSensor ultraSensor(PORT_10);
MeEncoderOnBoard motor1(SLOT1);
MeEncoderOnBoard motor2(SLOT2);

const int BUFFER_SIZE = 128;
const int SPEED = 100;
const int STOP = 0;

char incomingMessage[BUFFER_SIZE];
char msg;

bool messageReceived = false;
bool isForward = false;
bool isBackward = false;
bool isRotate = false;
bool isStop = true;

void setup() {
  Serial.begin(9600);
  memset(incomingMessage, 0 , BUFFER_SIZE);
}

void loop() {
    handleSerialCommunication();
    motorController();
}

void handleSerialCommunication(){
  static int bufferIndex = 0;
  while(Serial.available() > 0){
    char incomingChar = (char)Serial.read();
    if(incomingChar == '\n' || bufferIndex >= BUFFER_SIZE -1){
      incomingMessage[bufferIndex] = '\0';
      messageReceived = true;
      bufferIndex = 0;
      sendConfirmationMessage();
      break;
    }
    else{
      incomingMessage[bufferIndex++] = incomingChar;
    }
  }
  if (messageReceived) {
      handleReceivedMessage();
      messageReceived = false;  }
}

void sendConfirmationMessage(){
  Serial.print(incomingMessage);
}

void handleReceivedMessage(){
  if(String(incomingMessage) == "F"){
    Serial.print("Motor ON");
    isForward = true;
    isBackward = false;
    isRotate = false;
    isStop = false;
  }
  if(String(incomingMessage) == "S"){
    Serial.print("Motor OFF");
    isForward = false;
    isBackward = false;
    isRotate = false;
    isStop = true;
  }
  if(String(incomingMessage) == "B"){
    isForward = false;
    isBackward = true;
    isRotate = false;
    isStop = true;
  }
  if(String(incomingMessage) == "U"){
    Serial.print(getUltrasonicDistance());
  }
}

void motorController(){
  if(isForward){
    motor1.setMotorPwm(SPEED);
    motor2.setMotorPwm(-SPEED);
  }
  if(isBackward){
    motor1.setMotorPwm(-SPEED);
    motor2.setMotorPwm(SPEED);
  }
  if(isStop){
    motor1.setMotorPwm(STOP);
    motor2.setMotorPwm(STOP);
  }
  if(isRotate){

  }
}

double getUltrasonicDistance() {
  return ultraSensor.distanceCm();
}

