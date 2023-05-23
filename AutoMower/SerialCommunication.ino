//This functions looks if there is any incoming data on the Serial-bus
void handleSerialCommunication() {
  while (Serial.available() > 0) {
    Serial.readBytesUntil('\n', incomingMessage, BUFFER_SIZE);
    handleReceivedMessage();
  }
}

//This function will compare the received message to an instruction. It basically handles the messages. Add more cases if you want the mower to hande more commands
void handleReceivedMessage() {
  char command = incomingMessage[0];
  switch (command) {
    case 'X':
        Encoder_1.updateCurPos();
        encoderValue_1 = Encoder_1.getCurPos();
        Serial.print(encoderValue_1);
      break;
    case 'Y':
        Encoder_2.updateCurPos();
        encoderValue_2 = Encoder_2.getCurPos();
        Serial.print(encoderValue_2);
      break;
    case 'A':
      drivingMode = AUTO;
      break;
    case 'M':
      drivingMode = MANUALL;
      break;
    case 'F':
      manDirection = GO_FORWARD;
        playStarWarsTheme();
      break;
    case 'D':
      char colorStatic = incomingMessage[1];
      switch (colorStatic) {
        case 'R':
          setLEDLoop(RED);
          break;
        case 'G':
          setLEDLoop(GREEN);
          break;
        case 'B':
          setLEDLoop(BLUE);
          break;
        case 'W':
          setLEDLoop(WHITE);
          break;
      }
      break;
    case 'd':
      char colorBlink = incomingMessage[1];
      switch (colorBlink) {
        case 'R':
          setLEDLoopBlink(RED, 1000);
          break;
        case 'G':
          setLEDLoopBlink(GREEN, 1000);
          break;
        case 'B':
          setLEDLoopBlink(BLUE, 1000);
          break;
        case 'W':
          setLEDLoopBlink(WHITE, 1000);
          break;
        case 'b':
          setLEDLoopBlink(BLUETOOTHCOLOR, 1000);
          break;
      }
      break;
  }
}