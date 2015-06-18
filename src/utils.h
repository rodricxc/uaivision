#ifndef UTILS_H
#define UTILS_H

#include "QImage"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

using namespace cv;
using namespace std;

class Utils
{
public:
    Utils();

    static QImage mat2QImage(const Mat& mat);
    static Scalar bgr2hsv(Scalar rgb);
};

#endif // UTILS_H
