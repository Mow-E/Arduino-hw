//returns what the "eyes"/ gray-scale sensors sees, it can be either 0,1,2 or 3 
int getLightSensor() {
  return lineFollower.readSensors();
}

//Returns the distance in cm
double getUltrasonicDistance() {
  return ultraSensor.distanceCm();
}

//Sets the LED-ring in a static color
void setLEDLoop(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i <= 12; i++) {
    rgbLED.setColorAt(i, r, g, b);
    rgbLED.show();
  }
}

//Sets the LED-ring in a blinking color with 1s delay between "color/off"
void setLEDLoopBlink(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 3; i++) {
    setLEDLoop(r, g, b);
    delay(1000);
    setLEDLoop(BLACK);
    delay(1000);
  }
}