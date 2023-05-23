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
void setLEDLoopBlink(uint8_t r, uint8_t g, uint8_t b, uint8_t time) {
  for (int i = 0; i < 3; i++) {
    setLEDLoop(r, g, b);
    delay(time);
    setLEDLoop(BLACK);
    delay(time);
  }
}
void playTone(int t) {
  for (int i = 0; i < t; i++) {
    buzzer.tone(BUZZ_PORT, 440, 500);
    buzzer.noTone();
  }
}

void playStarWarsTheme() {
 int melody[] = {392, 392, 392, 311, 494, 392, 311, 494, 392, 587, 587, 587, 622, 494, 392, 311, 494, 392};
int noteDurations[] = {500, 500, 500, 350, 150, 500, 350, 150, 500, 500, 500, 500, 350, 150, 500, 350, 150, 500};

for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
  buzzer.tone(BUZZ_PORT, melody[i], noteDurations[i]);
  delay(noteDurations[i]);
  buzzer.noTone(BUZZ_PORT); // stop the sound after the note duration
  delay(50); // a little delay between notes
}

}