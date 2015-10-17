#ifndef SELECTBORDER_H
#define SELECTBORDER_H

#include <QWidget>

#include <iostream>
#include <configdao.h>
#include <QPainter>
#include <QPixmap>


namespace Ui {
  class selectBorder;
}

class SelectBorder : public QWidget
{
  Q_OBJECT

public:
  explicit SelectBorder(QWidget *parent = 0);
  ~SelectBorder();

  void loadUaiSoccerLogo();
  void loadImage(QString filePath);
  void loadImage(QImage img);
public slots:
  void labelClicked(QPoint pos);
  void displayImageSLOT(QImage image);
private slots:
  void on_saveButton_clicked();

private:
  Ui::selectBorder *ui;
  QPoint clickPosition;
  void drawPoint(QPoint point, QPixmap *pixmap);
  void drawLine(QPoint pA, QPoint pB, QPixmap *pixmap);
};

#endif // SELECTBORDER_H
