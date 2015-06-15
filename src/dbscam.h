#ifndef DBSCAM_H
#define DBSCAM_H

#include "aruco/aruco.h"
#include "vector"
#include "stdlib.h"
#include "math.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"

using namespace cv;
using namespace std;
using namespace aruco;

class DBSCAM {

private:
    vector<Scalar> data;



public:
    DBSCAM();

    static double distancePixels(Scalar a, Scalar b);
};

#endif // DBSCAM_H
