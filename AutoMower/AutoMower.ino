
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

MeUltrasonicSensor ultraSensor(PORT_8);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);

const int BUFFER_SIZE = 128;
const int SPEED = 100;

unsigned long previousMillis = 0;  // Stores the last time the action was performed
unsigned interval = 800;           // Interval at which the action should be performed (in milliseconds)
unsigned long currentMillis = 0;

char incomingMessage[BUFFER_SIZE] = { 0 };

enum MowerMode {
  AUTO,
  MANUALL,
  COLLISION
};
enum CollisionMode {
  TO_CLOSE,
  LINE_LEFT,
  LINE_RIGHT
};
enum MANUALLMODE {
  GO_FORWARD,
  GO_BACK,
  GO_RIGHT,
  GO_LEFT,
  GO_STOP
};

MowerMode drivingMode;
CollisionMode collisionReason;
MANUALLMODE manDirection;



void setup() {

  drivingMode = MANUALL;
  manDirection = GO_STOP;
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
  switch (drivingMode) {
    case MANUALL:
    //Serial.println("Manuall");
      setLEDLoop(GREEN);
      manuall();
      break;
    case AUTO:
    //Serial.println("AUTO");
      setLEDLoop(BLUE);
      autonomous();
      break;
    case COLLISION:
    //Serial.println("Collision");
      setLEDLoop(RED);
      collisionHandler();
      break;
  }
}

void manuall() {
  switch (manDirection) {
    case GO_FORWARD:
      move(FORWARD, SPEED);
      break;
    case GO_BACK:
      move(BACK, SPEED);
      break;
    case GO_STOP:
      move(STOP, 0);
      break;
    case GO_LEFT:
      move(LEFT, SPEED);
      break;
    case GO_RIGHT:
      move(RIGHT, SPEED);
      break;
  }
}
void autonomous() {
  if (isOkArea()) {
    //isTooClose = lineIsLeftSide = lineIsRightSide = false;
    move(FORWARD, SPEED);
  }
  //NOT OK AREA
  else {
    //isCollision = true;
    //isAuto = isMan = false;
    drivingMode = COLLISION;
    if (isTooCloseToObject()) {
      //isTooClose = true;
      collisionReason = TO_CLOSE;
    }
    switch (getLightSensor()) {
      case 0:
      case 1:
        //lineIsLeftSide = true;
        //lineIsRightSide = false;
        collisionReason = LINE_RIGHT;
        break;
      case 2:
        collisionReason = LINE_LEFT;
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
