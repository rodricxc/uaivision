#ifndef MAINQTHREAD_H
#define MAINQTHREAD_H

#include "QThread"
#include "iostream"
#include "QImage"
#include "utils.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "camera.h"
#include "aruco/aruco.h"
#include "aruco/cvdrawingutils.h"
#include "aruco/marker.h"
#include "stdlib.h"
#include "vector"
#include "math.h"
#include "QTime"
#include "calibpoint.h"

using namespace cv;
using namespace std;
using namespace aruco;


class MainQThread : public QThread {
    Q_OBJECT
public:
    MainQThread();


    vector<CalibPoint> getCalibData() const;

signals:
    void displayThisImage(QImage image);
    void displayThisImageMin(QImage image);
    void sendCalibData();

private:

    void getSubTrapezoid(Mat &src, Mat &out, Point a, Point b, Point c, Point d);


    vector<CalibPoint> getRandomPixels(Mat &image, int numPoits, Marker A, Marker B, Marker C);
    vector<CalibPoint> getRandomPixels(Mat &image, Mat &grayImage, int numPoits);
    vector<CalibPoint> calibData;

protected:
    virtual void run();
};

#endif // MAINQTHREAD_H
