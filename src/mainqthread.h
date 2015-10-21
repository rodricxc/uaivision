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
#include "configdao.h"
#include <QColor>
#include <robot.h>

using namespace cv;
using namespace std;
using namespace aruco;


class MainQThread : public QThread {
    Q_OBJECT
public:
    MainQThread();


    vector<CalibPoint> getCalibData() const;
    void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
    void drawObject(int x, int y,Mat &frame);
    Point pointFromQPoint(QPoint p, double inWidth, double inHeight, double outWidth, double outHeight);
    void drawField(Mat &image, Scalar color);
    Point2d pixelToCm(Point2d proportion, Point2d pixelPos);
    Point2d cmToPixelProportionPoint(Mat &img);
    double euclidianDistance(Point2d a, Point2d b);
    QColor scalarToQColor(Scalar c);
    Point2d cmToPixelPoint(Point2d proportion, Point2d cmPos);
    Scalar qColorToScalar(QColor c);
signals:
    void displayThrashold(QImage image);
    void displayThisImage(QImage image);
    void displayThisImageMin(QImage image);
    void displayImageSelectBorders(QImage image);
    void sendCalibData();

public slots:
    void getThresholdSpace();


private:

    void getSubTrapezoid(Mat &src, Mat &out, Point a, Point b, Point c, Point d);


    vector<CalibPoint> getRandomPixels(Mat &image, int numPoits, Marker A, Marker B, Marker C);
    vector<CalibPoint> getRandomPixels(Mat &image, Mat &grayImage, int numPoits);
    vector<CalibPoint> calibData;

    Scalar minHSV;
    Scalar maxHSV;

    void removeBorders(Mat &src, Mat &out, QPoint topLeft, QPoint bottomLeft, QPoint bottomRight, QPoint topRight, int width, int height);
    vector<Point>  trackFilteredObject2(Mat threshold, Mat &cameraFeed, Scalar c);
    void markers();
    //void cmToPixel(Mat &img, double x, double y, int &px, int &py);
   // void cmToPixelProportion(Mat &img, double &px, double &py);
    vector<Point> trackFilteredObject2Ball(Mat threshold, Mat &cameraFeed, Scalar c);
    void drawRobot(Mat &image, Robot robot, Point2d proportion);
    void cmToPixelProportion(Mat &img, double &px, double &py);
    void cmToPixel(Mat &img, double x, double y, int &px, int &py);
protected:
    virtual void run();
};

#endif // MAINQTHREAD_H
