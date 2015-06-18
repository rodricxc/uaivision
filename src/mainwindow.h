#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainqthread.h"
#include "glwindow.h"
#include "QGraphicsScene"
#include "QLabel"
#include "QTime"

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


private slots:
    void on_actionGLColors_triggered();
    void addData();

private:
    QLabel *frameRateLabel;
    QTime frameRateTime;
    double frameRate;
    void frameRateConfig();
    void setFrameRateLabel(double rate);

    GLWindow *glWindow;

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
