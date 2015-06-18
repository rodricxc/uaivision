#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QWidget>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;


namespace Ui {
class GLWindow;
}

class GLWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GLWindow(QWidget *parent = 0);
    ~GLWindow();


    void addData(vector<Scalar> v);


private:
    Ui::GLWindow *ui;
};

#endif // GLWINDOW_H
