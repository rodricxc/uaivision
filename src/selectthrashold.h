#ifndef SELECTTHRASHOLD_H
#define SELECTTHRASHOLD_H

#include <QWidget>
#include <QListWidgetItem>
#include <colorspace.h>

namespace Ui {
  class selectThrashold;
}

class SelectThrashold : public QWidget
{
  Q_OBJECT

public:
  explicit SelectThrashold(QWidget *parent = 0);
  ~SelectThrashold();

signals:
  void changedIntervalThrashold();

public slots:
  void sendInterval(int t);

  void displayImageSLOT(QImage image);
private slots:
  void on_colorDisplay_clicked();

  void on_pushButton_clicked();



  void on_listWidget_itemSelectionChanged();

  void on_colorName_textChanged(const QString &arg1);

  void on_pushButton_2_clicked();

private:
  int loaded;
  QColor color;
  Ui::selectThrashold *ui;
  void updateSpacesList();
  void carregarColorSpace(ColorSpace cs);
  ColorSpace insert();
  QString currentItem;
  ColorSpace *currentColorSpace();
  ColorSpace atualColorSpace();
  void updateColorSpace(ColorSpace cs, QString nome);
  void loadImage(QImage img);
  void loadImage(QString filePath);
  void loadUaiSoccerLogo();
};

#endif // SELECTTHRASHOLD_H
