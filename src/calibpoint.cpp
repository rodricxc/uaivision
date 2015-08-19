#include "calibpoint.h"

CalibPoint::CalibPoint() {

}

CalibPoint::CalibPoint(Scalar rgb, Scalar hsv) {
    this->hsv = hsv;
    this->rgb = rgb;
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



