#include "videolabel.h"

VideoLabel::VideoLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent,f) {

}

VideoLabel::VideoLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):QLabel(text,parent,f) {

}

VideoLabel::~VideoLabel() {

}

void VideoLabel::mousePressEvent(QMouseEvent *ev) {
  emit clicked(ev->pos());
}
