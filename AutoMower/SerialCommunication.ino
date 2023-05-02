void handleSerialCommunication() {
  while (Serial.available() > 0) {
    Serial.readBytesUntil('\n', incomingMessage, BUFFER_SIZE);
    handleReceivedMessage();
  }
}

void handleReceivedMessage() {
  char command = incomingMessage[0];
  switch (command) {
    case 'A':
      isAuto = true;
      isMan = false;
      break;
    case 'M':
      isAuto, isCollision = false;
      isMan = true;
      break;
    case 'F':
      isForward = true;
      isBackward = isRight = isLeft = isStop = false;
      break;
    case 'S':
      isStop = true;
      isBackward = isRight = isLeft = isForward = false;
      break;
    case 'B':
      isBackward = true;
      isForward = isRight = isLeft = isStop = false;
      break;
    case 'R':
      isRight = true;
      isBackward = isForward = isLeft = isStop = false;
      break;
    case 'L':
      isLeft = true;
      isBackward = isRight = isForward = isStop = false;
      break;
    case 'U':
      Serial.print(getUltrasonicDistance());
      break;
    case 'E':
      Serial.print(getLightSensor());
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
          setLEDLoopBlink(RED);
          break;
        case 'G':
          setLEDLoopBlink(GREEN);
          break;
        case 'B':
          setLEDLoopBlink(BLUE);
          break;
        case 'W':
          setLEDLoopBlink(WHITE);
          break;
      }
      break;
  }
}
