#include "selectborder.h"
#include "ui_selectborder.h"

using namespace std;

SelectBorder::SelectBorder(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::selectBorder) {
  ui->setupUi(this);

  connect(ui->videoLabel, SIGNAL(clicked(QPoint)), this, SLOT(labelClicked(QPoint)));


  ConfigDAO *config = ConfigDAO::Instance();

  this->loadUaiSoccerLogo();


  this->ui->width->setValue(config->getFieldWidth());
  this->ui->height->setValue(config->getFieldHeight());
  this->ui->border->setValue(config->getFieldBorder());
  this->ui->goalDepth->setValue(config->getFieldGoalDepth());
  this->ui->goalSize->setValue(config->getFieldGoalSize());

}

SelectBorder::~SelectBorder() {
  delete ui;
}


void SelectBorder::displayImageSLOT(QImage image) {
  loadImage(image);
}

void SelectBorder::loadUaiSoccerLogo() {

  this->loadImage("../src/images/UAISoccer.png");
}

void SelectBorder::loadImage(QString filePath) {
  QPixmap image;
  if(image.load(filePath)){
      ui->videoLabel->setPixmap(image.scaled(640,480));
    } else {
      cout <<"error on loading from file '" << filePath.toStdString() << "'" << endl;
    }
}

void SelectBorder::loadImage(QImage img) {
  QPixmap image = QPixmap::fromImage(img.scaled(640,480));

  ConfigDAO *config = ConfigDAO::Instance();

  this->drawPoint(config->getCornerLeftTop(), &image);
  this->drawPoint(config->getCornerLeftBottom(), &image);
  this->drawPoint(config->getCornerRightTop(), &image);
  this->drawPoint(config->getCornerRightBottom(), &image);

  this->drawLine(config->getCornerRightBottom(), config->getCornerRightTop(), &image);
  this->drawLine(config->getCornerRightTop(),    config->getCornerLeftTop(), &image);
  this->drawLine(config->getCornerLeftTop(),     config->getCornerLeftBottom(), &image);
  this->drawLine(config->getCornerLeftBottom(),  config->getCornerRightBottom(),  &image);


  ui->videoLabel->setPixmap(image);
}

void SelectBorder::labelClicked(QPoint pos) {
  ConfigDAO *config = ConfigDAO::Instance();
  if (this->ui->radioTL->isChecked()) {
      config->setCornerLeftTop(pos);
    }

  if (this->ui->radioTR->isChecked()) {
      config->setCornerRightTop(pos);
    }

  if (this->ui->radioBL->isChecked()) {
      config->setCornerLeftBottom(pos);
    }

  if (this->ui->radioBR->isChecked()) {
      config->setCornerRightBottom(pos);
    }




 // clickPosition = pos;
  cout << "click at: "<< pos.x() << ", "<< pos.y() << endl;
}

void SelectBorder::drawPoint(QPoint point, QPixmap *pixmap) {

  if(point.x()>=0) {
      QPainter p(pixmap);
      QBrush brush2( Qt::red);
      p.setBrush(brush2);
      p.drawEllipse(point, 3, 3);
      p.end();
    }
}


void SelectBorder::drawLine(QPoint pA,QPoint pB, QPixmap *pixmap) {

  if(pA.x()>=0 && pB.x()>=0) {
      QPainter p(pixmap);
      QPen mypen( Qt::red);
      mypen.setWidth(1);
      p.setPen(mypen);
      p.drawLine(pA,pB);
      p.end();
    }
}

void SelectBorder::on_saveButton_clicked() {
  ConfigDAO::Instance()->save();
}

void SelectBorder::on_width_valueChanged(int arg1) {
    ConfigDAO::Instance()->setFieldWidth(arg1);
}

void SelectBorder::on_height_valueChanged(int arg1) {
    ConfigDAO::Instance()->setFieldHeight(arg1);
}

void SelectBorder::on_border_valueChanged(int arg1) {
    ConfigDAO::Instance()->setFieldBorder(arg1);
}

void SelectBorder::on_goalSize_valueChanged(int arg1) {
    ConfigDAO::Instance()->setFieldGoalSize(arg1);
}

void SelectBorder::on_goalDepth_valueChanged(int arg1) {
    ConfigDAO::Instance()->setFieldGoalDepth(arg1);
}
