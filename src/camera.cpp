#include "camera.h"

Camera* Camera::camera = NULL;

Camera::Camera() {
    startVideoCapture();
    this->reloaded = false;

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
   //*
    this->videoCapture->read(im);
    if (im.empty()) {
      this->reloaded = true;
      this->stopVideoCapture();
      this->startVideoCapture();
      this->videoCapture->read(im);

    }
    //*/

   // im = imread("transformed.png", CV_LOAD_IMAGE_COLOR);
}

void Camera::startVideoCapture() {
  videoCapture = new VideoCapture("/home/rodricxc/uaivision/19.mp4");
  //videoCapture = new VideoCapture("/home/rodricxc/uaivision/7.mp4");
  //videoCapture = new VideoCapture("/home/rodricxc/cam.mp4");
    //videoCapture = new VideoCapture("/var/www/filme/ski.mp4");
    //videoCapture = new VideoCapture(0);
    //std::cout << "start Camera";
}

void Camera::stopVideoCapture() {
    delete videoCapture;
}

bool Camera::reload() {
  return this->reloaded;
}
