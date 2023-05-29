#include <MeAuriga.h>

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

//#define BUZZ_PORT 45

//Defined the different component-objects
MeUltrasonicSensor ultraSensor(PORT_8);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower lineFollower(PORT_9);
MeRGBLed rgbLED(0, 12);
//MeBuzzer buzzer;

const long BAUD_RATE = 115200;

const int BUFFER_SIZE = 128;  // The size of how long a serial-message can be stored
const int SPEED = 120;

const int WAIT_TIME = 1000;
const int PICTURE_TIME = 2000;
const int BACK_TIME = 1000;
const int ROTATE_TIME = 900;
const int DONE_TIME = 500;

unsigned long previousMillis = 0;  // Stores the last time the action was performed
unsigned interval = 1000;          // This is the time for how long a  "collision-action" will be performed in (ms).
unsigned long currentMillis = 0;

// ska det var unsigned long? och int bara vanlig long?
long encoderValue_1 = 0;
long encoderValue_2 = 0;

char incomingMessage[BUFFER_SIZE] = { 0 };  //This is the created buffer of serial-communication

//Declared the 3 different "main"-modes that the mower can be in
enum MowerMode {
  AUTO,
  MANUALL
};

//These two functions handles the interupt-serices requests for the two motors
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

//This functions sets the direction of the motors
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
  Encoder_1.setMotorPwm(leftSpeed);
  Encoder_2.setMotorPwm(rightSpeed);

  Encoder_1.loop();
  Encoder_2.loop();
}

MowerMode state = MANUALL;
char characterFromRaspberryPI;

void setup() {
  // put your setup code here, to run once:
  //manDirection = GO_STOP;
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(1);
  //rgbLED.setpin(44);
/*
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
*/
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}

void loop() {
  currentMillis = millis();
  if(Serial.available() > 0){
    characterFromRaspberryPI = Serial.read();
    if (characterFromRaspberryPI == 'A'){
      state = AUTO;
    }
    else{
      state = MANUALL;
    }
  }

  switch (state){
  case AUTO:
    if(ultraSensor.distanceCm() <= MAX_DISTANCE){
      /*
      Då är den för nära:
      STANNA
      1. Skicka ett 'C' till raspberryPI
      2. Skicka kordinaterna
      3. Backa
      4. Rotera
      5. kör autonomous igen.
      */
      move(STOP,0);
      delay(100);
      move(BACK,SPEED);
      delay(BACK_TIME);
      move(STOP,0);
      Serial.print("C");
      Serial.print(" ");
      Serial.print(encoderValue_1);
      Serial.print(" ");
      Serial.print(encoderValue_2);
      Serial.println();
      delay(PICTURE_TIME);
      move(RIGHT, SPEED);
      delay(ROTATE_TIME);
    }
    else if(lineFollower.readSensors() == 0 || lineFollower.readSensors() == 1 || lineFollower.readSensors() == 2){
      /*
      Kör över en border:
      STANNA
      1. Skicka ett 'P'
      2. Skicka Koordinater
      3. Backa
      4. Rotera
      5. Kör autonomous igen.
      */
      move(STOP,0);
      Serial.print("P");
      Serial.print(" ");
      Serial.print(encoderValue_1);
      Serial.print(" ");
      Serial.print(encoderValue_2);
      Serial.println();
      delay(500);
      move(BACK,SPEED);
      delay(BACK_TIME);
      move(STOP,0);
      delay(100);
      move(RIGHT, SPEED);
      delay(ROTATE_TIME);
    }
    // om det gått en sekund Skicka ett W
    else if ((currentMillis - previousMillis) > 1000){
      previousMillis = millis();
      Serial.print("W");
      Serial.print(" ");
      Serial.print(encoderValue_1);
      Serial.print(" ");
      Serial.print(encoderValue_2);
      Serial.println();
      
      /*
      WORKING:
      1. Skicka ett 'W'
      2. SKicka koordinater
      
      Serial.print("W");
      Serial.print(" ");
      Serial.print(encoderValue_1);
      Serial.print(" ");
      Serial.print(encoderValue_2);
      Serial.println();
      */
    
    }
    
    
    move(FORWARD, SPEED);
    break;
  
  case MANUALL:
    if ((currentMillis - previousMillis) > 1000){
      previousMillis = millis();
      Serial.print("W");
      Serial.print(" ");
      Serial.print(encoderValue_1);
      Serial.print(" ");
      Serial.print(encoderValue_2);
      Serial.println();
    }
    if (characterFromRaspberryPI == 'F')
    {
      move(FORWARD,SPEED);
    }
    else if (characterFromRaspberryPI == 'B')
    {
      move(BACK,SPEED);
    }
    else if (characterFromRaspberryPI == 'L')
    {
      move(LEFT,SPEED);
    }
    else if (characterFromRaspberryPI == 'R')
    {
      move(RIGHT,SPEED);
    }
    else if (characterFromRaspberryPI == 'S' || characterFromRaspberryPI == 'M' )
    {
      move(STOP,SPEED);
    }
    break;
  }
  
  Encoder_1.updateCurPos();
  Encoder_2.updateCurPos();
  encoderValue_1 = Encoder_1.getCurPos();
  encoderValue_2 = Encoder_2.getCurPos();

  // put your main code here, to run repeatedly:

}