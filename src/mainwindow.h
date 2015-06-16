#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainqthread.h"
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
    void stopmain();
    void startmain();
    void displayImageSLOT(QImage image);


private:
    QLabel *frameRateLabel;
    QTime frameRateTime;
    double frameRate;
    void frameRateConfig();
    void setFrameRate(double rate);

    void loadUaiSoccerLogo();
    void loadImage(QString filePath);
    void loadImage(QImage img);
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    MainQThread *main;

};

#endif // MAINWINDOW_H
