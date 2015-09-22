#include "colorspace.h"

ColorSpace::ColorSpace() {
  this->minHSV = Scalar(0,0,0);
  this->maxHSV = Scalar(255,255,255);
  this->color = Scalar(127,127,127);
  this->name = QString("Undefined");
}

ColorSpace::ColorSpace(Scalar min, Scalar max, Scalar color, QString name) {
  this->minHSV = min;
  this->maxHSV = max;
  this->color = color;
  this->name = name;
}

Scalar ColorSpace::getMinHSV() const {
  return minHSV;
}

void ColorSpace::setMinHSV(const Scalar &value) {
  minHSV = value;
}

Scalar ColorSpace::getMaxHSV() const {
  return maxHSV;
}

void ColorSpace::setMaxHSV(const Scalar &value) {
  maxHSV = value;
}

Scalar ColorSpace::getColor() const {
  return color;
}

void ColorSpace::setColor(const Scalar &value) {
  color = value;
}

QString ColorSpace::getName() const {
  return name;
}

void ColorSpace::setName(const QString &value) {
  name = value;
}





