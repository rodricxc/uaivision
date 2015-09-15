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

class DBSCAN {

private:
    vector<CalibPoint*> toAnalise;
    set<CalibPoint> points;
    vector< vector<CalibPoint*> > clusters;
    vector<CalibPoint*> outliers;
    int minPoints;
    double raio;

    int numOfNeighbors(CalibPoint *p);

    void insertToAnalise(CalibPoint *p);
    //void insertCluster(CalibPoint *p);
    void insertOutliers(CalibPoint *p);


public:
    void analise();
    DBSCAN();
    void insertPoint(CalibPoint p);
    void insertPointsVector(vector<CalibPoint> v);
    void clear();

    static double distancePixels(Scalar a, Scalar b);
    set<CalibPoint> getPoints() const;
    int getMinPoints() const;
    void setMinPoints(int value);
    double getRaio() const;
    void setRaio(double value);
};

#endif // DBSCAM_H
