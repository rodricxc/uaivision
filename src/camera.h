#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

using namespace cv;
using namespace std;

class Camera {
public:

    static Camera *getCamera();
    bool isOpened();
    void read(cv::Mat &im);

    bool reload();
private:
    static Camera *camera;
    VideoCapture *videoCapture;
    void startVideoCapture();
    void stopVideoCapture();
    bool reloaded;


    Camera();
};

#endif // CAMERA_H
