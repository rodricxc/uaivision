#include "calibpoint.h"

CalibPoint::CalibPoint() {
    this->classification = UNDEFINED;
}

CalibPoint::CalibPoint(Scalar rgb, Scalar hsv) {
    this->hsv = hsv;
    this->rgb = rgb;
    this->classification = UNDEFINED;
}

Scalar CalibPoint::getRgb() const {
    return rgb;
}

void CalibPoint::setRgb(const Scalar &value) {
    rgb = value;
}

Scalar CalibPoint::getHsv() const {
    return hsv;
}

void CalibPoint::setHsv(const Scalar &value) {
    hsv = value;
}

double CalibPoint::euclidianDistanceRGBFrom(CalibPoint p) {
    Scalar a = this->rgb;
    Scalar b = p.getRgb();
    return sqrt((a.val[0]-b.val[0])*(a.val[0]-b.val[0])+
                (a.val[1]-b.val[1])*(a.val[1]-b.val[1])+
                (a.val[2]-b.val[2])*(a.val[2]-b.val[2]));
}

double CalibPoint::euclidianDistanceHSVFrom(CalibPoint p) {
    Scalar a = this->hsv;
    Scalar b = p.getHsv();
    return sqrt((a.val[0]-b.val[0])*(a.val[0]-b.val[0])+
                (a.val[1]-b.val[1])*(a.val[1]-b.val[1])+
                (a.val[2]-b.val[2])*(a.val[2]-b.val[2]));
}

CalibPoint::PointClassification CalibPoint::getClassification() const {
  return classification;
}

void CalibPoint::toCore() {
  this->classification = CORE;
}

void CalibPoint::toOutlier() {
  this->classification = OUTLIER;
}

void CalibPoint::toUndefined() {
  this->classification = UNDEFINED;
}

void CalibPoint::toBorder() {
  this->classification = BORDER;
}

bool CalibPoint::isCore() {
  return this->classification == CORE;
}

bool CalibPoint::isOutlier() {
  return this->classification == OUTLIER;
}

bool CalibPoint::isUndefined() {
  return this->classification == UNDEFINED;
}

bool CalibPoint::isBorder() {
  return this->classification == BORDER;
}

bool CalibPoint::operator==(const CalibPoint &other) {
    Scalar otherHSV = other.getHsv();
    Scalar otherRGB = other.getRgb();
    return (this->rgb[0] == otherRGB[0] &&
            this->rgb[1] == otherRGB[1] &&
            this->rgb[2] == otherRGB[2]);
}

bool CalibPoint::operator!=(const CalibPoint &other) {
  return !(*this == other);
}

bool CalibPoint::operator<(const CalibPoint &other) const {
  Scalar otherHSV = other.getHsv();
  Scalar meHSV = this->getHsv();
  if (meHSV[0] == otherHSV[0]) {
      if (meHSV[1] == otherHSV[1]) {
          return meHSV[2] < otherHSV[2];
        } else {
          return meHSV[1] < otherHSV[1];
        }
   } else {
      return meHSV[0] < otherHSV[0];
   }
}





