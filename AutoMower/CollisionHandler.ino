
enum CollisionHandlingStates {
  WAIT,
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
          //WAIT FOR TAKE PICTURE (SEND PICTURE COMMAND)
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
  switch (avoidState){
    case HOLD:
      move(STOP,0);
      break;
    case RETREAT:
      move(BACK,SPEED);
      break;
    case GORIGHT:
      move(RIGHT, SPEED*1.6);
      break;
    case GOLEFT:
      move(LEFT, SPEED*1.6);
      break;
    case CONTINUE:
      //DO NOTHING STATE
      break;
  }
}