
#include "MeAuriga.h"

#define FORWARD 2
#define BACK 1
#define LEFT 3
#define RIGHT 4
#define ROTATIONFACTOR 1

#define red 255, 0, 0
#define green 0, 255, 0
#define blue 0, 0, 255
#define white 255, 255, 255
#define black 0, 0, 0


MeUltrasonicSensor ultraSensor(PORT_10);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);

const int BUFFER_SIZE = 128;
const int SPEED = 100;
const int STOP = 0;

int rotation = 0;

char incomingMessage[BUFFER_SIZE] = { 0 };

bool isForward = false;
bool isBackward = false;
bool isRotate = false;
bool isStop = true;

void setup() {
  Serial.begin(9600);
  rgbLED.setpin(44);

  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}

void loop() {
  handleSerialCommunication();
  motorController();
  //move(4,100);
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
  if (incomingMessage[0] == 'R') {
    int tempHundred = incomingMessage[1] - '0';
    int tempTen = incomingMessage[2] - '0';
    int tempDigit = incomingMessage[3] - '0';
    rotation = (tempHundred * 100) + (tempTen * 10) + (tempDigit);

    isForward = false;
    isBackward = false;
    isRotate = true;
    isStop = false;

    Serial.print(rotation);
  }
  if (incomingMessage[0] == 'r') {
    int tempHundred = incomingMessage[1] - '0';
    int tempTen = incomingMessage[2] - '0';
    int tempDigit = incomingMessage[3] - '0';
    rotation = ((tempHundred * 100) + (tempTen * 10) + (tempDigit)) * -1;

    isForward = false;
    isBackward = false;
    isRotate = true;
    isStop = false;

    Serial.print(rotation);
  }
  if (incomingMessage[0] == 'U') {
    Serial.print(getUltrasonicDistance());
  }
  if (incomingMessage[0] == 'E') {
    Serial.print(getLightSensor());
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
    move(FORWARD, SPEED);
  }
  if (isBackward) {
    move(BACK, SPEED);
  }
  if (isStop) {
    move(FORWARD, STOP);
  }
  if (isRotate) {
    if (rotation > 0) {
      move(LEFT, SPEED);
      // _delay(rotation * ROTATIONFACTOR);
      // move(LEFT, STOP);
      // isRotate = false;
    }
    if (rotation < 0) {
      move(RIGHT, SPEED);
      // _delay(rotation * ROTATIONFACTOR);
      // move(RIGHT, STOP);
      // isRotate = false;
    }
  }
}

double getUltrasonicDistance() {
  return ultraSensor.distanceCm();
}

void isr_process_encoder1(void) {
  if (digitalRead(Encoder_1.getPortB()) == 0) {
    Encoder_1.pulsePosMinus();
  } else {
    Encoder_1.pulsePosPlus();
  }
}
void isr_process_encoder2(void) {
  if (digitalRead(Encoder_2.getPortB()) == 0) {
    Encoder_2.pulsePosMinus();
  } else {
    Encoder_2.pulsePosPlus();
  }
}
void move(int direction, int speed) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  if (direction == BACK) {
    leftSpeed = -speed;
    rightSpeed = speed;
  } else if (direction == FORWARD) {
    leftSpeed = speed;
    rightSpeed = -speed;
  } else if (direction == LEFT) {
    leftSpeed = -speed;
    rightSpeed = -speed;
  } else if (direction == RIGHT) {
    leftSpeed = speed;
    rightSpeed = speed;
  }
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed);

  Encoder_1.loop();
  Encoder_2.loop();
}
void _delay(float seconds) {
  if (seconds < 0.0) {
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while (millis() < endTime)
    ;
}

int getLightSensor() {
  return lineFollower.readSensors();
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
