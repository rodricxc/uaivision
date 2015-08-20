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

    enum PointClassification {
        CORE,
        OUTLIER,
        BORDER,
        UNDEFINED
    };


    CalibPoint();
    CalibPoint(Scalar rgb, Scalar hsv);

    Scalar getRgb() const;
    void setRgb(const Scalar &value);

    Scalar getHsv() const;
    void setHsv(const Scalar &value);

    double euclidianDistanceRGBFrom(CalibPoint p);
    double euclidianDistanceHSVFrom(CalibPoint p);

    PointClassification getClassification() const;

    //overloading the equal (==) comparator
    bool operator==(const CalibPoint &other);
    bool operator!=(const CalibPoint &other);

private:
    Scalar rgb;
    Scalar hsv;
    PointClassification classification;

};

#endif // CALIBPOINT_H
