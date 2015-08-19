#include "glwindow.h"
#include "ui_glwindow.h"

GLWindow::GLWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GLWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked()), ui->widgetToGL, SLOT(clearData()));

}

GLWindow::~GLWindow() {
    delete ui;
}

void GLWindow::addData(vector<CalibPoint> v) {
    ui->widgetToGL->addData(v);
}

