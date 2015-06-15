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

using namespace cv;
using namespace std;
using namespace aruco;


class MainQThread : public QThread {
    Q_OBJECT
public:
    MainQThread();


signals:
    void displayThisImage(QImage image);

private:
    vector<Scalar> getRandomPixels(Mat &image, int numPoits, Marker A, Marker B, Marker C);


protected:
    virtual void run();
};

#endif // MAINQTHREAD_H
