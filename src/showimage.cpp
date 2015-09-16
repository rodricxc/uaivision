#include "showimage.h"
#include "ui_showimage.h"

using namespace std;

ShowImage::ShowImage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ShowImage) {
  ui->setupUi(this);

  this->loadUaiSoccerLogo();
}

ShowImage::~ShowImage() {
  delete ui;
}

void ShowImage::displayImageSLOT(QImage image) {
  loadImage(image);
}

void ShowImage::loadUaiSoccerLogo() {

  this->loadImage("../src/images/UAISoccer.png");
}

void ShowImage::loadImage(QString filePath) {
  QPixmap image;
  if(image.load(filePath)){
      ui->videoLabel->setPixmap(image.scaled(640,480,Qt::KeepAspectRatio));
    } else {
      cout <<"error on loading from file '" << filePath.toStdString() << "'" << endl;
    }
}

void ShowImage::loadImage(QImage img) {
  ui->videoLabel->setPixmap(QPixmap::fromImage(img.scaled(640,480,Qt::KeepAspectRatio)));
}
