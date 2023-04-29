
#include "MeAuriga.h"

#define FORWARD 2
#define BACK 1
#define LEFT 3
#define RIGHT 4
#define STOP 5

#define MAX_DISTANCE 5

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255
#define WHITE 255, 255, 255
#define BLACK 0, 0, 0

MeUltrasonicSensor ultraSensor(PORT_10);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);

const int BUFFER_SIZE = 128;
const int SPEED = 80;

char incomingMessage[BUFFER_SIZE] = { 0 };

bool isForward = false;
bool isBackward = false;
bool isRight = false;
bool isLeft = false;
bool isStop = true;

bool isAuto = false;
bool isMan = true;
bool isCollision = false;

bool isTooClose = false;
bool lineIsLeftSide = false;
bool lineIsRightSide = false;

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
  mowerState();
}

void mowerState() {
  if (isMan) {
    manuall();
  }
  if (isAuto) {
    autonomous();
  }
  if (isCollision) {
    collisionHandler();
  }
}

void manuall() {
  if (isForward) {
    move(FORWARD, SPEED);
  }
  if (isBackward) {
    move(BACK, SPEED);
  }
  if (isStop) {
    move(STOP, 0);
  }
  if (isLeft) {
    move(LEFT, SPEED);
  }
  if (isRight) {
    move(RIGHT, SPEED);
  }
}
void autonomous() {
  if (isOkArea()) {
    setLEDLoop(GREEN);
    // Serial.println("mowing...");
    isTooClose = lineIsLeftSide = lineIsRightSide = false;

    move(FORWARD, SPEED);
  }
  //NOT OK AREA
  else {
    isCollision = true;
    isAuto = isMan = false;
    if (isTooCloseToObject()) {
      Serial.println("TOO CLOSE");
      isTooClose = true;
    }
    switch (getLightSensor()) {
      case 0:
      case 1:
        lineIsLeftSide = true;
        lineIsRightSide = false;
        break;
      case 2:
        lineIsRightSide = true;
        lineIsLeftSide = false;
        break;
    }
  }
}
bool isOkArea() {
  return (getUltrasonicDistance() > MAX_DISTANCE && getLightSensor() == 3);
}
bool isTooCloseToObject() {
  return (getUltrasonicDistance() <= MAX_DISTANCE);
}
