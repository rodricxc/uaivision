#include "camera.h"

Camera::Camera() {
    startVideoCapture();
}

bool Camera::isOpened() {
    return this->videoCapture->isOpened();
}

void Camera::read(Mat &im) {
    this->videoCapture->read(im);
}

void Camera::startVideoCapture() {
    //videoCapture = new VideoCapture("/var/www/filme/ski.mp4");
    videoCapture = new VideoCapture(0);
    //std::cout << "start Camera";
}

void Camera::stopVideoCapture() {
    delete videoCapture;
}

