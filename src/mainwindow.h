#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainqthread.h"
#include "glwindow.h"
#include "QGraphicsScene"
#include "QLabel"
#include "QTime"
#include "showimage.h"
#include "selectthrashold.h"
#include "selectborder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void stopMain();
    void startMain();

    void displayImageSLOT(QImage image);
    void displayImageMinSLOT(QImage image);
    void getThresholdSpace(int minH,int minS,int minV,int maxH,int maxS,int maxV);


private slots:
    void on_actionGLColors_triggered();
    void on_actionThresholded_Image_triggered();
    void on_actionThrashold_Colors_triggered();
    void addData();

    void on_actionSelect_Borders_triggered();

private:
    QLabel *frameRateLabel;
    QTime frameRateTime;
    double frameRate;
    void frameRateConfig();
    void setFrameRateLabel(double rate);

    GLWindow *glWindow;
    ShowImage *thrasholded;
    SelectThrashold *selectThrashold;
    SelectBorder *selectBorder;

    Scalar minHSV;
    Scalar maxHSV;



    void loadUaiSoccerLogo();
    void loadImage(QString filePath);
    void loadImage(QImage img);

    void loadImageMin(QString filePath);
    void loadImageMin(QImage img);
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    MainQThread *main;

};

#endif // MAINWINDOW_H
