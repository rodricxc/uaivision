#include "robot.h"

Robot::Robot() {

}

Robot::Robot(QColor teamColor, QColor robotColor, Point2d robotPosition, double robotAngle) {
  this->colorTeam = teamColor;
  this->colorRobot = robotColor;
  this->position = robotPosition;
  this->angle = robotAngle;
}

QColor Robot::getColorTeam() const {
  return colorTeam;
}

void Robot::setColorTeam(const QColor &value) {
  colorTeam = value;
}

QColor Robot::getColorRobot() const {
  return colorRobot;
}

void Robot::setColorRobot(const QColor &value) {
  colorRobot = value;
}

Point2d Robot::getPosition() const {
  return position;
}

void Robot::setPosition(const Point2d &value) {
  position = value;
}

double Robot::getAngle() const {
  return angle;
}

void Robot::setAngle(double value) {
  angle = value;
}





