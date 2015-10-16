#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QImageReader"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    frameRateConfig();

    this->glWindow = new GLWindow();
    this->thrasholded = new ShowImage();
    this->selectThrashold = new SelectThrashold();
    //glWindow->setVisible(true);
    main  = new MainQThread();
    scene = new QGraphicsScene(this);
    ui->videoLabel->setMargin(1);

    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(stopMain()));
    connect(ui->start, SIGNAL(pressed()), this, SLOT(startMain()));
    connect(main, SIGNAL(displayThisImage(QImage)), this, SLOT(displayImageSLOT(QImage)));
    connect(main, SIGNAL(displayThisImageMin(QImage)), this, SLOT(displayImageMinSLOT(QImage)));
    connect(main, SIGNAL(sendCalibData()), this, SLOT(addData()));

    connect(main, SIGNAL(displayThrashold(QImage)), thrasholded, SLOT(displayImageSLOT(QImage)));
    connect(selectThrashold, SIGNAL(changedIntervalThrashold()), main, SLOT(getThresholdSpace()));


    loadUaiSoccerLogo();
    QApplication::setWindowIcon(QIcon("../src/images/icon.png"));

}
MainWindow::~MainWindow() {
    delete main;
    delete scene;
    delete glWindow;
    delete ui;
}

void MainWindow::stopMain() {
    main->requestInterruption();
    main->wait();
    std::cout << "saiu"<< std::endl;
    loadUaiSoccerLogo();
    setFrameRateLabel(0);
}


void MainWindow::startMain() {
    frameRateTime = QTime::currentTime();
    main->start();
    std::cout << "comecou"<< std::endl;
}

void MainWindow::displayImageSLOT(QImage image) {

   // cout << "width: " << image.width() << "  height: " << image.height() <<endl;
    loadImage(image);
    frameRate = frameRate*0.97 + 0.03*(1/(frameRateTime.msecsTo(QTime::currentTime())/1000.0));
    setFrameRateLabel(frameRate);
    frameRateTime = QTime::currentTime();
}

void MainWindow::displayImageMinSLOT(QImage image) {
  loadImageMin(image);
}

void MainWindow::getThresholdSpace(int minH, int minS, int minV, int maxH, int maxS, int maxV) {
  this->minHSV = Scalar(minH,minS,minV);
  this->maxHSV = Scalar(maxH,maxS,maxV);

  cout << "set min and max " << minH << endl;
}

void MainWindow::frameRateConfig() {
    frameRateLabel = new QLabel("Frame rate: 0.00 fps", this);
    statusBar()->addPermanentWidget(frameRateLabel, 40);
    frameRateTime = QTime::currentTime();
}

void MainWindow::setFrameRateLabel(double rate) {
    this->frameRateLabel->setText("Frame rate: "+ QString::number(rate,'f', 2)+" fps");
}

void MainWindow::loadUaiSoccerLogo() {
    loadImage("../src/images/UAISoccer.png");
    loadImageMin("../src/images/UAISoccer.png");
}

void MainWindow::loadImage(QString filePath) {
    QPixmap image;
    if(image.load(filePath)){
        ui->videoLabel->setPixmap(image.scaled(640,480,Qt::KeepAspectRatio));
    } else{
        cout <<"error on loading from file '" << filePath.toStdString() << "'" << endl;
    }
}

void MainWindow::loadImage(QImage img) {
    ui->videoLabel->setPixmap(QPixmap::fromImage(img.scaled(640,480,Qt::KeepAspectRatio)));
}

void MainWindow::loadImageMin(QString filePath) {
    QPixmap image;
    if(image.load(filePath)){
        ui->videoLabelMin->setPixmap(image.scaled(320,240,Qt::KeepAspectRatio));
    } else{
        cout <<"error on loading to Min Image from file '" << filePath.toStdString() << "'" << endl;
    }
}

void MainWindow::loadImageMin(QImage img) {
    ui->videoLabelMin->setPixmap(QPixmap::fromImage(img.scaled(320,240,Qt::KeepAspectRatio)));
}

void MainWindow::on_actionGLColors_triggered() {
    this->glWindow->setVisible(true);
}

void MainWindow::on_actionThresholded_Image_triggered() {
  this->thrasholded->setVisible(true);
}

void MainWindow::on_actionThrashold_Colors_triggered() {
  this->selectThrashold->setVisible(true);
}

void MainWindow::addData() {
    this->glWindow->addData(main->getCalibData());
}
