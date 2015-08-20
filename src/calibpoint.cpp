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

bool CalibPoint::operator==(const CalibPoint &other) {
    Scalar otherHSV = other.getHsv();
    Scalar otherRGB = other.getRgb();
    return (this->hsv[0] == otherHSV[0] &&
            this->hsv[1] == otherHSV[1] &&
            this->hsv[2] == otherHSV[2] &&
            this->rgb[0] == otherRGB[0] &&
            this->rgb[1] == otherRGB[1] &&
            this->rgb[2] == otherRGB[2]);
}

bool CalibPoint::operator!=(const CalibPoint &other) {
    return !(*this == other);
}




