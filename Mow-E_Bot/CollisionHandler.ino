void collisionHandler() {
  if (isTooClose) {
    stop();
    moveBack();
  }
  if (lineIsLeftSide) {
    stop();
    moveBack();
    turnRight();
  }
  if (lineIsRightSide) {
    stop();
    moveBack();
    turnLeft();
  }
}
void stop() {
}
void moveBack() {
}
void turnLeft() {
}
void turnRight() {
}