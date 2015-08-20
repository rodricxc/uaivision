#include "dbscam.h"

void DBSCAM::insertToAnalise(CalibPoint *p) {
    this->toAnalise.insert(this->toAnalise.end(), p);
}

void DBSCAM::insertOuliers(CalibPoint *p) {
    this->outliers.insert(this->outliers.end(), p);
}

DBSCAM::DBSCAM() {

}

void DBSCAM::insertPoint(CalibPoint p) {
    vector<CalibPoint>::iterator it = this->points.end();
    this->points.insert( this->points.end(), p);
    for (;it != this->points.end(); ++it) {
        insertToAnalise((CalibPoint*) &(*it));
    }
}

void DBSCAM::insertPointsVector(vector<CalibPoint> v) {
    vector<CalibPoint>::iterator it = this->points.end();
    this->points.insert( this->points.end(), v.begin(), v.end());
    for (;it != this->points.end(); ++it) {
         insertToAnalise((CalibPoint*) &(*it));
    }
}

double DBSCAM::distancePixels(Scalar a, Scalar b) {
    return sqrt((a.val[0]-b.val[0])*(a.val[0]-b.val[0])+
                (a.val[1]-b.val[1])*(a.val[1]-b.val[1])+
                (a.val[2]-b.val[2])*(a.val[2]-b.val[2]));
}

