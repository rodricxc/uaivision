#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QtOpenGL/qgl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <QTimer>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "stdlib.h"
#include "vector"
#include "math.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>

using namespace cv;
using namespace std;

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);



    void addData(vector<Scalar> v);
    void addData(Scalar p);

    double getRotateX() const;
    void setRotateX(double value);

    double getRotateY() const;
    void setRotateY(double value);

    double getRotateZ() const;
    void setRotateZ(double value);

public slots:
    void clearData();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void drawPixel(Scalar p);
    void drawInData();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

   // virtual void keyPressEvent(QKeyEvent *);

private:

    double rotateX;
    double rotateY;
    double rotateZ;


    QTimer timer;

    vector<Scalar> inData;

    QPoint lastPos;


};

#endif // MYGLWIDGET_H
