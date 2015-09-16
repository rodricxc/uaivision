#ifndef SHOWIMAGE_H
#define SHOWIMAGE_H

#include <QWidget>
#include <iostream>

namespace Ui {
  class ShowImage;
}

class ShowImage : public QWidget
{
  Q_OBJECT

public:
  explicit ShowImage(QWidget *parent = 0);
  ~ShowImage();

public slots:
  void displayImageSLOT(QImage image);

private:
  void loadUaiSoccerLogo();
  void loadImage(QString filePath);
  void loadImage(QImage img);
  Ui::ShowImage *ui;
};

#endif // SHOWIMAGE_H
