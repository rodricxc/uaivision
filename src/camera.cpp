#include "camera.h"

Camera* Camera::camera = NULL;

Camera::Camera() {
    startVideoCapture();
}

Camera* Camera::getCamera() {
  if (camera == NULL) {
      camera = new Camera();
    }
  return camera;
}

bool Camera::isOpened() {
    return this->videoCapture->isOpened();
}

void Camera::read(Mat &im) {
    this->videoCapture->read(im);
    if (im.empty()) {
      this->stopVideoCapture();
      this->startVideoCapture();
      this->videoCapture->read(im);

    }
}

void Camera::startVideoCapture() {
  videoCapture = new VideoCapture("/home/rodricxc/uaivision/9.mp4");
  //videoCapture = new VideoCapture("/home/rodricxc/uaivision/7.mp4");
  //videoCapture = new VideoCapture("/home/rodricxc/cam.mp4");
    //videoCapture = new VideoCapture("/var/www/filme/ski.mp4");
    //videoCapture = new VideoCapture(0);
    //std::cout << "start Camera";
}

void Camera::stopVideoCapture() {
    delete videoCapture;
}

