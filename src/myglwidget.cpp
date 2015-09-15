#include "myglwidget.h"
#include "iostream"
#include "utils.h"
using namespace cv;
using namespace std;

MyGLWidget::MyGLWidget(QWidget *parent): QGLWidget(parent) {
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(1000);

    setRotateX(0);
    setRotateY(0);
    setRotateZ(0);
    // addData(Scalar(200,0,0));
    //  addData(Scalar(100,0,0));
    //  addData(Scalar(50,0,0));
    //  addData(Scalar(150,0,0));
    this->dbscan = new DBSCAN();

}

void MyGLWidget::addData(vector<CalibPoint> v) {
  //inData.clear();
    this->dbscan->insertPointsVector(v);

    set<CalibPoint> p = this->dbscan->getPoints();
    inData.clear();
    inData.insert( inData.end(), p.begin(), p.end());

    cout << "numero de pontos inseridos: " << v.size() << endl;
}

void MyGLWidget::addData(CalibPoint p) {
    inData.insert( inData.end(), p);

}

void MyGLWidget::clearData() {
    this->dbscan->clear();
  this->inData.clear();
}

void MyGLWidget::clusterize() {
    this->dbscan->analise();
}

void MyGLWidget::initializeGL() {
    glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity();
    gluLookAt(20, 10, 20,   0, 0, 0,    0, 1, 0);


    glRotatef(getRotateX(), 1, 0, 0);
    glRotatef(getRotateY(), 0, 1, 0);
    glRotatef(getRotateZ(), 0, 0, 1);


    glBegin(GL_LINES); {
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(10,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,10,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,10);


        glColor3f(1,1,1);
        glVertex3f(0,0,10);
        glVertex3f(0,10,10);


        glColor3f(1,1,1);
        glVertex3f(10,0,0);
        glVertex3f(10,0,10);

        glColor3f(1,1,1);
        glVertex3f(10,0,0);
        glVertex3f(10,10,0);


        glColor3f(1,1,1);
        glVertex3f(10,10,0);
        glVertex3f(10,10,10);


        glColor3f(1,1,1);
        glVertex3f(10,0,10);
        glVertex3f(10,10,10);


        glColor3f(1,1,1);
        glVertex3f(0,10,10);
        glVertex3f(10,10,10);

        glColor3f(1,1,1);
        glVertex3f(0,0,10);
        glVertex3f(10,0,10);

        glColor3f(1,1,1);
        glVertex3f(0,10,0);
        glVertex3f(10,10,0);

        glColor3f(1,1,1);
        glVertex3f(0,10,0);
        glVertex3f(0,10,10);


    }
    glEnd();
/*
    glColor3f(1, 0.6, 0);
    glPushMatrix();
    // glTranslatef(4,3,1);
    // //glutSolidSphere(0.1, 6, 6);
    glPopMatrix();
*/


    drawInData();


}

void MyGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20, 10, 20,   5, 5, 5,    0, 1, 0);

    /*
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glOrtho(0, width, height, 0, -1, -1);

    glMatrixMode(GL_MODELVIEW);

*/

}

void MyGLWidget::drawPixel(CalibPoint p) {

    Scalar hsv = p.getHsv();
    Scalar bgr = p.getRgb();
    glPushMatrix();
    float x = ((float)hsv.val[2]/255.0);
    float y = ((float)hsv.val[1]/255.0);
    float z = ((float)hsv.val[0]/255.0);

    float x_color = ((float)bgr.val[2]/255.0);
    float y_color = ((float)bgr.val[1]/255.0);
    float z_color = ((float)bgr.val[0]/255.0);


    glColor3f(x_color,y_color,z_color);
    //glTranslatef(10.0*x_, 10.0*y_, 10.0*z_);
    glTranslatef(10.0*x, 10.0*y, 10.0*z);
    glutSolidSphere(0.05, 6, 6);

    //std::cout << "x: " << x <<" y: "<< y << " z: " << z <<std::endl;

    glPopMatrix();
}

void MyGLWidget::drawInData() {

    vector<CalibPoint>::iterator it;

    for (it = inData.begin(); it!=inData.end(); it++){
        if ((it)->isCore()) {
            drawPixel(*it);
        }
    }

}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();


}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    double alfa  = 0.3;
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setRotateX(getRotateX() + alfa * dy);
        setRotateY(getRotateY() + alfa * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setRotateX(getRotateX() + alfa * dy);
        setRotateZ(getRotateY() + alfa * dx);
    }

    lastPos = event->pos();
    update();
}
double MyGLWidget::getRotateZ() const {
    return rotateZ;
}

void MyGLWidget::setRotateZ(double value) {
    rotateZ = value;
    while (rotateZ > 360) {rotateZ -= 360; }
    while (rotateZ < 0) {rotateZ += 360; }
}

double MyGLWidget::getRotateY() const {
    return rotateY;
}

void MyGLWidget::setRotateY(double value) {
    rotateY = value;
    while (rotateY > 360) {rotateY -= 360; }
    while (rotateY < 0) {rotateY += 360; }

}

void MyGLWidget::setRotateX(double value) {
    rotateX = value;
    while (rotateX > 360) {rotateX -= 360; }
    while (rotateX < 0) {rotateX += 360; }
}

double MyGLWidget::getRotateX() const {
    return rotateX;
}


