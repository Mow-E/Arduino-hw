
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
  if ((currentMillis - previousMillis) >= interval) {
    if (isTooClose) {
      switch (currentState) {
        case WAIT:
          Serial.print("Wait ->");
          avoidState = HOLD;
          previousMillis = currentMillis;
          currentState = MOVEBACK;
          break;
        case MOVEBACK:
          Serial.print("Moves back ->");
          avoidState = RETREAT;
          previousMillis = currentMillis;
          currentState = TAKEPICTURE;
          break;
        case TAKEPICTURE:
          Serial.print("Take Picture ->");
          //WAIT FOR TAKE PICTURE (SEND PICTURE COMMAND)
          avoidState = HOLD;
          previousMillis = currentMillis;
          currentState = TURNRIGHT;
          break;
        case TURNRIGHT:
          Serial.print("Turns right ->");
          avoidState = GORIGHT;
          previousMillis = currentMillis;
          currentState = DONE;
          break;
        case DONE:
          Serial.println("Done");
          avoidState = CONTINUE;
          isCollision = false;
          isAuto = true;
          previousMillis = currentMillis;
          currentState = WAIT;
          break;
      }
    }
    if (lineIsLeftSide) {
      switch (currentState) {
        case WAIT:
          Serial.print("Wait ->");
          avoidState = HOLD;
          previousMillis = currentMillis;
          currentState = MOVEBACK;
          break;
        case MOVEBACK:
          Serial.print("Moves back ->");
          avoidState = RETREAT;
          previousMillis = currentMillis;
          currentState = TURNRIGHT;
          break;
        case TURNRIGHT:
          Serial.print("Turns right ->");
          avoidState = GORIGHT;
          previousMillis = currentMillis;
          currentState = DONE;
          break;
        case DONE:
          Serial.println("Done");
          avoidState = CONTINUE;
          isCollision = false;
          isAuto = true;
          previousMillis = currentMillis;
          currentState = WAIT;
          break;
      }
    }
    if (lineIsRightSide) {
      switch (currentState) {
        case WAIT:
          Serial.print("Wait ->");
          avoidState = HOLD;
          previousMillis = currentMillis;
          currentState = MOVEBACK;
          break;
        case MOVEBACK:
          Serial.print("Moves back ->");
          avoidState = RETREAT;
          previousMillis = currentMillis;
          currentState = TURNLEFT;
          break;
        case TURNLEFT:
          Serial.print("Turns left ->");
          avoidState = GOLEFT;
          previousMillis = currentMillis;
          currentState = DONE;
          break;
        case DONE:
          Serial.println("Done");
          avoidState = CONTINUE;
          isCollision = false;
          isAuto = true;
          previousMillis = currentMillis;
          currentState = WAIT;
          break;
      }
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