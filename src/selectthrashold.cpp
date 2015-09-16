#include "selectthrashold.h"
#include "ui_selectthrashold.h"

SelectThrashold::SelectThrashold(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::selectThrashold)
{
  ui->setupUi(this);

  connect(ui->s_max_h, SIGNAL(sliderMoved(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_max_s, SIGNAL(sliderMoved(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_max_v, SIGNAL(sliderMoved(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_min_h, SIGNAL(sliderMoved(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_min_s, SIGNAL(sliderMoved(int)), this, SLOT(sendInterval(int)));
  connect(ui->s_min_v, SIGNAL(sliderMoved(int)), this, SLOT(sendInterval(int)));

}

SelectThrashold::~SelectThrashold() {
  delete ui;
}

void SelectThrashold::sendInterval(int t) {
  int minH = ui->s_min_h->value();
  int minS = ui->s_min_s->value();
  int minV = ui->s_min_v->value();
  int maxH = ui->s_max_h->value();
  int maxS = ui->s_max_s->value();
  int maxV = ui->s_max_v->value();
  emit changeIntervalThrashold(minH, minS, minV, maxH, maxS, maxV);
}
