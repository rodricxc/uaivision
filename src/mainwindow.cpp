#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QImageReader"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    frameRateConfig();


    main  = new MainQThread();
    scene = new QGraphicsScene(this);
    ui->videoLabel->setMargin(1);

    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(stopmain()));
    connect(ui->start, SIGNAL(pressed()), this, SLOT(startmain()));
    connect(main, SIGNAL(displayThisImage(QImage)), this, SLOT(displayImageSLOT(QImage)));

    loadUaiSoccerLogo();
    QApplication::setWindowIcon(QIcon("../src/images/icon.png"));
    //statusBar()->showMessage(tr("Ready", "not ready",300));
    //main->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::stopmain() {
    main->requestInterruption();
    main->wait();
    std::cout << "saiu"<< std::endl;
    loadUaiSoccerLogo();
    setFrameRate(0);
}


void MainWindow::startmain() {
    frameRateTime = QTime::currentTime();
    main->start();
    std::cout << "comecou"<< std::endl;
}

void MainWindow::displayImageSLOT(QImage image) {

   // cout << "width: " << image.width() << "  height: " << image.height() <<endl;
    loadImage(image);
    frameRate = 1/(frameRateTime.msecsTo(QTime::currentTime())/1000.0);
    setFrameRate(frameRate);
    frameRateTime = QTime::currentTime();
}

void MainWindow::frameRateConfig() {
    frameRateLabel = new QLabel("Frame rate: 0.00 fps", this);
    statusBar()->addPermanentWidget(frameRateLabel, 40);
    frameRateTime = QTime::currentTime();
}

void MainWindow::setFrameRate(double rate) {
    this->frameRateLabel->setText("Frame rate: "+ QString::number(rate,'f', 2)+" fps");
}

void MainWindow::loadUaiSoccerLogo() {
    loadImage("../src/images/UAISoccer.png");
}

void MainWindow::loadImage(QString filePath) {
    QPixmap image;
    if(image.load(filePath)){
        ui->videoLabel->setPixmap(image);
    } else{
        cout <<"error on loading from file '" << filePath.toStdString() << "'" << endl;
    }
}

void MainWindow::loadImage(QImage img) {
    ui->videoLabel->setPixmap(QPixmap::fromImage(img));
}
