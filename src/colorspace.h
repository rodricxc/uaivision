#ifndef COLORSPACE_H
#define COLORSPACE_H

#include <QObject>
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class ColorSpace {

public:
  explicit ColorSpace();

  explicit ColorSpace(Scalar min, Scalar max, Scalar color, QString name);

  Scalar getMinHSV() const;
  void setMinHSV(const Scalar &value);

  Scalar getMaxHSV() const;
  void setMaxHSV(const Scalar &value);

  Scalar getColor() const;
  void setColor(const Scalar &value);

  QString getName() const;
  void setName(const QString &value);

private:
  Scalar minHSV;
  Scalar maxHSV;
  Scalar color;
  QString name;

};

#endif // COLORSPACE_H
