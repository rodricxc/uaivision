#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

using namespace cv;
using namespace std;

class Camera {
public:

    static Camera getCamera() {
        static Camera camera;
        return camera;
    }
    bool isOpened();
    void read(cv::Mat &im);

private:

    VideoCapture *videoCapture;
    void startVideoCapture();
    void stopVideoCapture();


    Camera();
};

#endif // CAMERA_H
