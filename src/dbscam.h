#ifndef DBSCAM_H
#define DBSCAM_H

#include "aruco/aruco.h"
#include "vector"
#include "stdlib.h"
#include "calibpoint.h"
#include "math.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"

using namespace cv;
using namespace std;
using namespace aruco;

class DBSCAM {

private:
    vector<CalibPoint*> toAnalise;
    vector<CalibPoint> points;
    vector< vector<CalibPoint*> > clusters;
    vector<CalibPoint*> outliers;

    void insertToAnalise(CalibPoint *p);
    //void insertCluster(CalibPoint *p);
    void insertOuliers(CalibPoint *p);

public:
    DBSCAM();
    void insertPoint(CalibPoint p);
    void insertPointsVector(vector<CalibPoint> v);

    static double distancePixels(Scalar a, Scalar b);
};

#endif // DBSCAM_H
