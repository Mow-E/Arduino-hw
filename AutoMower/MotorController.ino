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
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed);

  Encoder_1.loop();
  Encoder_2.loop();
}

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
