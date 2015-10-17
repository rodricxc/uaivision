#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H
#include <QLabel>
#include <QMouseEvent>


class VideoLabel : public QLabel {
  Q_OBJECT

public:
  VideoLabel(QWidget *parent = 0, Qt::WindowFlags f=0);
  VideoLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
  ~VideoLabel();

protected:
  void mousePressEvent(QMouseEvent *ev);

signals:

  void clicked(QPoint pos);
};

#endif // VIDEOLABEL_H
