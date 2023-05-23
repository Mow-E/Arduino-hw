
#include "MeAuriga.h"

//Defined the different directions the motors can turn to
#define FORWARD 2
#define BACK 1
#define LEFT 3
#define RIGHT 4
#define STOP 5

#define MAX_DISTANCE 5  //This is the maximum distance a mower can be to an object, before it avoids it. In (cm), change this if you want to go closer or avoid earlier.

//Defined colors for the RGB-LED ring that is mounted on top of the mower.
#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255
#define WHITE 255, 255, 255
#define BLACK 0, 0, 0

#define AUTOCOLOR 144, 146, 189
#define MANUALCOLOR 47, 57, 99
#define BLUETOOTHCOLOR 36, 182, 85
#define ALARMCOLOR 173, 39, 39
#define BACKWARDCOLOR 242, 99, 28

#define BUZZ_PORT 45

//Defined the different component-objects
MeUltrasonicSensor ultraSensor(PORT_8);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);
MeBuzzer buzzer;

const long BAUD_RATE = 115200;

const int BUFFER_SIZE = 128;  // The size of how long a serial-message can be stored
const int SPEED = 100;        // The default speed of the mower (100/255), increase this if you want the mower to go faster in all directions

const int WAIT_TIME = 1000;
const int PICTURE_TIME = 3000;
const int BACK_TIME = 1000;
const int ROTATE_TIME = 2000;
const int DONE_TIME = 500;

unsigned long previousMillis = 0;  // Stores the last time the action was performed
unsigned interval = 1000;          // This is the time for how long a  "collision-action" will be performed in (ms).
unsigned long currentMillis = 0;

unsigned long encoderValue_1 = 0;
unsigned long encoderValue_2 = 0;

char incomingMessage[BUFFER_SIZE] = { 0 };  //This is the created buffer of serial-communication

//Declared the 3 different "main"-modes that the mower can be in
enum MowerMode {
  AUTO,
  MANUALL,
  COLLISION
};
//Declared the different Collision mode (To close to an object, a black line is either at left or right of the mower)
enum CollisionMode {
  TO_CLOSE,
  LINE_LEFT,
  LINE_RIGHT
};
//Declared the different modes(directions) in "Driving mode: Manuall"
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


// The setup() function will set the Default states and properties when the Mower is powered on.
void setup() {


  drivingMode = MANUALL;
  manDirection = GO_STOP;
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(1);
  rgbLED.setpin(44);

  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}

//The main loop looks at incoming serial-communication and handles that if there is a new message. The Mowestate() function will perform the correct action that corresponds to that received message
void loop() {
  handleSerialCommunication();
  mowerState();
}

//This is a state-machine that sets the mower in different driving modes: Manuall, Auto or in Collision (Avoid stuff).
void mowerState() {
  switch (drivingMode) {
    case MANUALL:
      setLEDLoop(GREEN);
      //setLEDLoopBlink(MANUALCOLOR, 2000);
      manuall();
      break;
    case AUTO:
      setLEDLoop(AUTOCOLOR);
      autonomous();
      break;
    case COLLISION:
      //Serial.println("Collision");
      setLEDLoop(ALARMCOLOR);
      collisionHandler();
      break;
  }
}

//State-machine for the "Manuall-mode", the mower can only be in one "state": go forward, backwards, stop or rotate to right and left.
void manuall() {
  switch (manDirection) {
    case GO_FORWARD:
      move(FORWARD, SPEED);
      break;
    case GO_BACK:
      setLEDLoop(BACKWARDCOLOR);
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

//This is one of the "main"-modes the mower can be in, this mode makes the mower go autonmously and therefore can drive and perform actions by it self.
void autonomous() {
  if (isOkArea()) {
    currentMillis = millis();
    if (((currentMillis - previousMillis) >= WAIT_TIME)) {
      previousMillis = currentMillis;
      Serial.print("W");   
      move(FORWARD, SPEED);    
    }
    else{   
      move(FORWARD, SPEED); 
    }
  }

  //NOT OK AREA
  else {
    drivingMode = COLLISION;
    if (isTooCloseToObject()) {
      collisionReason = TO_CLOSE;
    }
    switch (getLightSensor()) {
      case 0:
      case 1:
        collisionReason = LINE_RIGHT;
        break;
      case 2:
        collisionReason = LINE_LEFT;
        break;
    }
  }
}

//Returns a "TRUE" if the mowers is inside an ok-area and and not to close to an object, if "FALSE" the mower will go into "collision-mode"
bool isOkArea() {
  return (getUltrasonicDistance() > MAX_DISTANCE && getLightSensor() == 3);
}

//Checks if the mower is far away from an object that is in front of it. Returns true if it is to close, the mower will go into "collision-mode"
bool isTooCloseToObject() {
  return (getUltrasonicDistance() <= MAX_DISTANCE);
}
