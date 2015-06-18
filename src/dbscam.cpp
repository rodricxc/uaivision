#include "dbscam.h"

DBSCAM::DBSCAM() {

}

double DBSCAM::distancePixels(Scalar a, Scalar b) {
    return sqrt((a.val[0]-b.val[0])*(a.val[0]-b.val[0])+
                (a.val[1]-b.val[1])*(a.val[1]-b.val[1])+
                (a.val[2]-b.val[2])*(a.val[2]-b.val[2]));
}

