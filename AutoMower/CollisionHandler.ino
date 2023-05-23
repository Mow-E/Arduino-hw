
enum CollisionHandlingStates {
  WAIT,
  TAKEPICTURE,
  MOVEBACK,
  TURNRIGHT,
  TURNLEFT,
  DONE
};
enum AvoidMoves {
  HOLD,
  RETREAT,
  GORIGHT,
  GOLEFT,
  CONTINUE
};
CollisionHandlingStates currentState;
AvoidMoves avoidState;
void collisionHandler() {
  currentMillis = millis();
  if (((currentMillis - previousMillis) >= interval)) {
    switch (collisionReason) {
      case TO_CLOSE:
        switch (currentState) {
          case WAIT:
            interval = WAIT_TIME;
            avoidState = HOLD;
            previousMillis = currentMillis;
            currentState = MOVEBACK;
            break;
          case MOVEBACK:
            interval = BACK_TIME;
            avoidState = RETREAT;
            previousMillis = currentMillis;
            currentState = TAKEPICTURE;
            break;
          case TAKEPICTURE:
            //WAIT FOR TAKE PICTURE (SEND PICTURE COMMAND)
            interval = PICTURE_TIME;
            Serial.print("C");
            avoidState = HOLD;
            previousMillis = currentMillis;
            currentState = TURNRIGHT;
            break;
          case TURNRIGHT:
            interval = ROTATE_TIME;
            avoidState = GORIGHT;
            previousMillis = currentMillis;
            currentState = DONE;
            break;
          case DONE:
            interval = DONE_TIME;
            avoidState = CONTINUE;
            drivingMode = AUTO;
            previousMillis = currentMillis;
            currentState = WAIT;
            break;
        }
        break;
      case LINE_LEFT:
        switch (currentState) {
          case WAIT:
            interval = WAIT_TIME;
            Serial.print("P");
            avoidState = HOLD;
            previousMillis = currentMillis;
            currentState = MOVEBACK;
            break;
          case MOVEBACK:
            interval = BACK_TIME;
            avoidState = RETREAT;
            previousMillis = currentMillis;
            currentState = TURNRIGHT;
            break;
          case TURNRIGHT:
            interval = ROTATE_TIME;
            avoidState = GORIGHT;
            previousMillis = currentMillis;
            currentState = DONE;
            break;
          case DONE:
            interval = DONE_TIME;
            avoidState = CONTINUE;
            drivingMode = AUTO;
            previousMillis = currentMillis;
            currentState = WAIT;
            break;
        }
        break;
      case LINE_RIGHT:
        switch (currentState) {
          case WAIT:
            interval = WAIT_TIME;
            Serial.print("P");
            avoidState = HOLD;
            previousMillis = currentMillis;
            currentState = MOVEBACK;
            break;
          case MOVEBACK:
            avoidState = RETREAT;
            interval = BACK_TIME;
            previousMillis = currentMillis;
            currentState = TURNLEFT;
            break;
          case TURNLEFT:
            interval = ROTATE_TIME;
            avoidState = GOLEFT;
            previousMillis = currentMillis;
            currentState = DONE;
            break;
          case DONE:
            interval = DONE_TIME;
            avoidState = CONTINUE;
            drivingMode = AUTO;
            previousMillis = currentMillis;
            currentState = WAIT;
            break;
        }
        break;
    }
  }
  switch (avoidState) {
    case HOLD:
      move(STOP, 0);
      break;
    case RETREAT:
      move(BACK, SPEED);
      break;
    case GORIGHT:

      move(RIGHT, SPEED);
      break;
    case GOLEFT:
      move(LEFT, SPEED);
      break;
    case CONTINUE:
      //DO NOTHING STATE
      break;
  }
}