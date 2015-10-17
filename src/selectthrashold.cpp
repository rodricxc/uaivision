#include "selectthrashold.h"
#include "ui_selectthrashold.h"
#include <QColorDialog>
#include <QMessageBox>
#include "configdao.h"
#include "colorspace.h"
#include "iostream"

SelectThrashold::SelectThrashold(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::selectThrashold)
{
  ui->setupUi(this);
  this->loaded = 0;

  connect(ui->s_max_h, SIGNAL(valueChanged(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_max_s, SIGNAL(valueChanged(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_max_v, SIGNAL(valueChanged(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_min_h, SIGNAL(valueChanged(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_min_s, SIGNAL(valueChanged(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_min_v, SIGNAL(valueChanged(int)), this, SLOT(sendInterval(int)));

  const QString COLOR_STYLE("QPushButton { background-color : %1;}");

  this->color = QColor(255,255,255);

  ui->colorDisplay->setStyleSheet(COLOR_STYLE.arg(this->color.name()));
  ui->colorDisplay->update();

  this->updateSpacesList();
  this->loadUaiSoccerLogo();
}

SelectThrashold::~SelectThrashold() {
  delete ui;
}

void SelectThrashold::sendInterval(int t) {
  emit changedIntervalThrashold();
 // std::cout << "mandou" << std::endl;
  updateColorSpace(this->atualColorSpace(), this->currentItem);
}


void SelectThrashold::on_colorDisplay_clicked() {
  this->color = QColorDialog::getColor(Qt::white, this);
  const QString COLOR_STYLE("QPushButton { background-color : %1;}");

  ui->colorDisplay->setStyleSheet(COLOR_STYLE.arg(this->color.name()));
  updateColorSpace(this->atualColorSpace(), this->currentItem);
}

void SelectThrashold::on_pushButton_clicked() {
  carregarColorSpace(insert());
}

ColorSpace SelectThrashold::insert() {
  ConfigDAO *config = ConfigDAO::Instance();

  ColorSpace space =config->insertColorSpace(ColorSpace( Scalar(0,0,0),Scalar(0,0,0), Scalar(255,255,255), QString("New")));
  carregarColorSpace(space);
  this->updateSpacesList();
  return space;
}

void SelectThrashold::updateSpacesList() {
  ConfigDAO *config = ConfigDAO::Instance();
  vector<ColorSpace> colorSpaces = config->getColorSpaces();
  vector<ColorSpace>::iterator it = colorSpaces.begin();
   ui->listWidget->clearSelection();
   ui->listWidget->clearFocus();
   ui->listWidget->clear();

  for (; it!= colorSpaces.end(); it++) {
      ui->listWidget->addItem(it->getName());

    }
  ui->listWidget->update();
}

void SelectThrashold::carregarColorSpace(ColorSpace cs){
  this->loaded = 1;
  this->color.setRed(int(cs.getColor()[0]));
  this->color.setGreen(int(cs.getColor()[1]));
  this->color.setBlue(int(cs.getColor()[2]));
  this->ui->colorName->setText( cs.getName());
  this->currentItem = cs.getName();
  const QString COLOR_STYLE("QPushButton { background-color : %1;}");
  ui->colorDisplay->setStyleSheet(COLOR_STYLE.arg(this->color.name()));

  //std::cout << this->color.name().toStdString() << std::endl;

  ui->colorDisplay->update();

  this->ui->s_max_h->setValue(cs.getMaxHSV()[0]);
  this->ui->s_max_s->setValue(cs.getMaxHSV()[1]);
  this->ui->s_max_v->setValue(cs.getMaxHSV()[2]);

  this->ui->s_min_h->setValue(cs.getMinHSV()[0]);
  this->ui->s_min_s->setValue(cs.getMinHSV()[1]);
  this->ui->s_min_v->setValue(cs.getMinHSV()[2]);

}

ColorSpace SelectThrashold::atualColorSpace(){

  int minH = ui->s_min_h->value();
  int minS = ui->s_min_s->value();
  int minV = ui->s_min_v->value();
  int maxH = ui->s_max_h->value();
  int maxS = ui->s_max_s->value();
  int maxV = ui->s_max_v->value();

  ColorSpace space = ColorSpace(
        Scalar(minH, minS, minV),
        Scalar(maxH, maxS, maxV),
        Scalar(this->color.red(),this->color.green(),this->color.blue()),
        this->ui->colorName->text());

  return space;

}

void SelectThrashold::on_listWidget_itemSelectionChanged() {
  QListWidgetItem *item;
  item = (*(ui->listWidget->selectedItems().begin()));
  if (item) {
    ConfigDAO *config = ConfigDAO::Instance();
    vector<ColorSpace> colorSpaces = config->getColorSpaces();
    vector<ColorSpace>::iterator it = colorSpaces.begin();

    for (; it!= colorSpaces.end(); it++) {
      if (it->getName().compare(item->text())==0) {
          this->carregarColorSpace((*it));
          return;
        }
      }
  }
}

ColorSpace *SelectThrashold::currentColorSpace() {
  ConfigDAO *config = ConfigDAO::Instance();
  vector<ColorSpace> colorSpaces = config->getColorSpaces();
  vector<ColorSpace>::iterator it = colorSpaces.begin();
  for (; it!= colorSpaces.end(); it++) {
    if (it->getName().compare(this->currentItem)==0) {
        this->carregarColorSpace((*it));
        return &(*it);
      }
    }
 return NULL;
}

void SelectThrashold::updateColorSpace(ColorSpace cs, QString nome) {

  ConfigDAO *config = ConfigDAO::Instance();

  config->updateColorSpace(cs, nome);

}



void SelectThrashold::on_colorName_textChanged(const QString &arg1) {
  if (this->loaded==0 && !arg1.isEmpty() && !this->currentItem.isEmpty()) {
      ColorSpace c = this->atualColorSpace();
      c.setName(this->ui->colorName->text());
      updateColorSpace(c, this->currentItem);
      this->currentItem = this->ui->colorName->text();
      updateSpacesList();
    }
  this->loaded = 0;
}

void SelectThrashold::on_pushButton_2_clicked() {
  ConfigDAO *config = ConfigDAO::Instance();

  config->save();
}


void SelectThrashold::displayImageSLOT(QImage image) {
  loadImage(image);
}

void SelectThrashold::loadUaiSoccerLogo() {

  this->loadImage("../src/images/UAISoccer.png");
}

void SelectThrashold::loadImage(QString filePath) {
  QPixmap image;
  if(image.load(filePath)){
      ui->videoLabel->setPixmap(image.scaled(640,480,Qt::KeepAspectRatio));
    } else {
      cout <<"error on loading from file '" << filePath.toStdString() << "'" << endl;
    }
}

void SelectThrashold::loadImage(QImage img) {
  ui->videoLabel->setPixmap(QPixmap::fromImage(img.scaled(640,480,Qt::KeepAspectRatio)));
}
