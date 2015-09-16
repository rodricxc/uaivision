#ifndef SELECTTHRASHOLD_H
#define SELECTTHRASHOLD_H

#include <QWidget>

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
  void changeIntervalThrashold(int,int,int,int,int,int);

public slots:
  void sendInterval(int t);

private:
  Ui::selectThrashold *ui;
};

#endif // SELECTTHRASHOLD_H
