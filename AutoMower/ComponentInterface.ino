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