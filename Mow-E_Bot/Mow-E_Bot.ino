
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

const int TIME = 10;


MeUltrasonicSensor ultraSensor(PORT_10);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);

const int BUFFER_SIZE = 128;
const int SPEED = 80;

int rotation = 0;

char incomingMessage[BUFFER_SIZE] = { 0 };

bool isForward = false;
bool isBackward = false;
bool isRotate = false;
bool isStop = true;
bool isAuto = false;

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
}

void handleSerialCommunication() {
  while (Serial.available() > 0) {
    Serial.readBytesUntil('\n', incomingMessage, BUFFER_SIZE);
    handleReceivedMessage();
  }
}

void handleReceivedMessage() {
  if (incomingMessage[0] == 'A') {
    Serial.println("AUTOMODE");
    isForward = false;
    isBackward = false;
    isRotate = false;
    isStop = false;
    isAuto = true;
  }
  if (incomingMessage[0] == 'F') {
    isForward = true;
    isBackward = false;
    isRotate = false;
    isStop = false;
    isAuto = false;
  }
  if (incomingMessage[0] == 'S') {
    isForward = false;
    isBackward = false;
    isRotate = false;
    isStop = true;
    isAuto = false;
  }
  if (incomingMessage[0] == 'B') {
    isForward = false;
    isBackward = true;
    isRotate = false;
    isStop = false;
    isAuto = false;
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
    isAuto = false;
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
    isAuto = false;
  }
  if (incomingMessage[0] == 'U') {
    Serial.print(getUltrasonicDistance());
  }
  if (incomingMessage[0] == 'E') {
    Serial.print(getLightSensor());
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'R') {
    setLEDLoop(RED);
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'G') {
    setLEDLoop(GREEN);
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'B') {
    setLEDLoop(BLUE);
  }
  if (incomingMessage[0] == 'L' && incomingMessage[1] == 'W') {
    setLEDLoop(WHITE);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'R') {
    setLEDLoopBlink(RED);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'G') {
    setLEDLoopBlink(GREEN);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'B') {
    setLEDLoopBlink(BLUE);
  }
  if (incomingMessage[0] == 'l' && incomingMessage[1] == 'W') {
    setLEDLoopBlink(WHITE);
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
    move(STOP, 0);
  }
  if (isRotate) {
    if (rotation > 0) {
      move(LEFT, SPEED);
    }
    if (rotation < 0) {
      move(RIGHT, SPEED);
    }
  }
  if (isAuto) {
    autonomous();
  }
}

void move(int direction, int speed) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  if (direction == FORWARD) {
    leftSpeed = speed;
    rightSpeed = -speed;
  } else if (direction == BACK) {
    leftSpeed = -speed;
    rightSpeed = speed;
  } else if (direction == LEFT) {
    leftSpeed = -speed;
    rightSpeed = -speed;
  } else if (direction == RIGHT) {
    leftSpeed = speed;
    rightSpeed = speed;
  } else if (direction == STOP) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed);

  Encoder_1.loop();
  Encoder_2.loop();
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
    setLEDLoop(BLACK);
    delay(1000);
  }
}

void autonomous() {
  if (getUltrasonicDistance() > MAX_DISTANCE && getLightSensor() == 3) {
    setLEDLoop(GREEN);
    // Serial.println("mowing...");
    move(FORWARD, SPEED);
  } else {
    //Serial.println("OUTSIDE");
    setLEDLoop(RED);
    moveUntil(STOP);
    if (getUltrasonicDistance() <= MAX_DISTANCE) {
      Serial.println("TOO CLOSE");
      //takePicture();
      moveUntil(BACK);
      moveUntil(RIGHT);
    }
    if (getLightSensor() != 3) {
      if (getLightSensor() == 1) {
        Serial.println("LEFT EYE CLOSED");
        moveUntil(BACK);
        moveUntil(LEFT);
      }
      if (getLightSensor() == 2) {
        Serial.println("RIGHT EYE CLOSED");
        moveUntil(BACK);
        moveUntil(RIGHT);
      } else {
        Serial.println("BOTH EYES CLOSED");
        moveUntil(BACK);
        moveUntil(RIGHT);
      }
    }
  }
}
void takePicture() {
  Serial.print("C");
}

void moveUntil(int DIRECTION) {
  for (int s = 0; s < TIME; s + 1) {
    move(DIRECTION, SPEED);
  }
  Serial.print("Done MOVING");
}
