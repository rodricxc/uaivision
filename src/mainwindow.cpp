#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QImageReader"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    main  = new MainQThread();
    scene = new QGraphicsScene(this);
    ui->videoLabel->setMargin(1);

    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(stopmain()));
    connect(ui->start, SIGNAL(pressed()), this, SLOT(startmain()));
    connect(main, SIGNAL(displayThisImage(QImage)), this, SLOT(displayImageSLOT(QImage)));

    loadUaiSoccerLogo();
    QApplication::setWindowIcon(QIcon("../src/images/icon.png"));
    //main->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stopmain()
{
    main->requestInterruption();
    main->wait();
    std::cout << "saiu"<< std::endl;
    loadUaiSoccerLogo();
}


void MainWindow::startmain()
{
    main->start();
    std::cout << "comecou"<< std::endl;
}

void MainWindow::displayImageSLOT(QImage image)
{

   // cout << "width: " << image.width() << "  height: " << image.height() <<endl;
    loadImage(image);

}

void MainWindow::loadUaiSoccerLogo()
{
    loadImage("../src/images/UAISoccer.png");
}

void MainWindow::loadImage(QString filePath)
{
    QPixmap image;
    if(image.load(filePath)){
        ui->videoLabel->setPixmap(image);

    } else{
        cout <<"error on loading from file '" << filePath.toStdString() << "'" << endl;
    }
}

void MainWindow::loadImage(QImage img)
{
    ui->videoLabel->setPixmap(QPixmap::fromImage(img));
}
