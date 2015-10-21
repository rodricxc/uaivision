#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QColor>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "configdao.h"


using namespace cv;

class Robot {

public:
  Robot();
  Robot(QColor teamColor, QColor robotColor, Point2d robotPosition, double robotAngle);

  QColor getColorTeam() const;
  void setColorTeam(const QColor &value);

  QColor getColorRobot() const;
  void setColorRobot(const QColor &value);

  Point2d getPosition() const;
  void setPosition(const Point2d &value);

  double getAngle() const;
  void setAngle(double value);

signals:

public slots:


private:
  QColor colorTeam;
  QColor colorRobot;
  Point2d position;
  double angle;



};

#endif // ROBOT_H
