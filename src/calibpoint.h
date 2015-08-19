#ifndef CALIBPOINT_H
#define CALIBPOINT_H

#include <QObject>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


class CalibPoint
{
public:
    CalibPoint();
    CalibPoint(Scalar rgb, Scalar hsv);

    Scalar getRgb() const;
    void setRgb(const Scalar &value);

    Scalar getHsv() const;
    void setHsv(const Scalar &value);

private:
    Scalar rgb;
    Scalar hsv;

};

#endif // CALIBPOINT_H
