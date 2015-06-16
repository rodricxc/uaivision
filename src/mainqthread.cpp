#include "mainqthread.h"

using namespace cv;
using namespace std;
using namespace aruco;

MainQThread::MainQThread() {
    srand (time(NULL));
}



vector<Scalar> MainQThread::getRandomPixels(Mat &image, int numPoits, Marker A, Marker B, Marker C) {
    vector<Scalar>  pixels;

    Point2f origin;
    Point2f vector_i, vector_j;


    origin = A.getCenter();

    vector_i.x = B.getCenter().x - origin.x;
    vector_i.y = B.getCenter().y - origin.y;

    vector_j.x = C.getCenter().x - origin.x;
    vector_j.y = C.getCenter().y - origin.y;



    for(int i = 0; i < numPoits; i++){
        float alfa = (rand() % 700 + 150)/1000.0;
        float beta = (rand() % 800 + 100)/1000.0;

        Point newPoint;
        newPoint.x = (int) (origin.x + alfa*vector_i.x + beta*vector_j.x);
        newPoint.y = (int) (origin.y + alfa*vector_i.y + beta*vector_j.y);

        circle(image, newPoint,1, Scalar(0,0,255),3);
        Scalar pix = image.at<uchar>(newPoint);
        pixels.insert(pixels.end(), pix);
    }

    return pixels;


}

void MainQThread::run() {

    Camera camera = Camera::getCamera();
    CameraParameters cp;
    QTime frameRateTime;
    frameRateTime= QTime::currentTime();

    if (!camera.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
        return;
    }

    if (!this->isInterruptionRequested()){

        Mat cameraFrame;
        camera.read(cameraFrame);

        while(!this->isInterruptionRequested()){
/*
            MarkerDetector MDetector;
            vector<Marker> Markers;

            Marker A, B, C;


            //Ok, let's detect
            MDetector.detect(cameraFrame,Markers);
            //for each marker, draw info and its boundaries in the image
            for (unsigned int i=0;i<Markers.size();i++) {
                //  cout<<Markers[i]<<endl;

                Markers[i].draw(cameraFrame,Scalar(0,0,255),2);
                if (cp.isValid()){
                    aruco::CvDrawingUtils::draw3dCube(cameraFrame,Markers[i],cp);
                }





            }






            if (Markers.size()>=4){

                for (unsigned int i=0;i<Markers.size();i++) {
                    if (Markers[i].id==50){
                        A = Markers[i];
                    }
                    if (Markers[i].id==52){
                        B = Markers[i];
                    }
                    if (Markers[i].id==53){
                        C = Markers[i];
                    }
                }

                getRandomPixels(cameraFrame,100,A,B,C);


                Point2f center1 =  Markers[0].getCenter();
                Point2f center2 =  Markers[1].getCenter();
                Point2f center3 =  Markers[2].getCenter();
                Point2f center4 =  Markers[3].getCenter();
                Point2f center1_ =  Markers[0].getCenter();
                Point2f center2_ =  Markers[1].getCenter();
                Point2f center3_ =  Markers[2].getCenter();
                Point2f center4_ =  Markers[3].getCenter();

                line(cameraFrame, center1, center4, Scalar(0,0,255),2);
                line(cameraFrame, center2, center4, Scalar(0,0,255),2);
                line(cameraFrame, center3, center2, Scalar(0,0,255),2);
                line(cameraFrame, center3, center1, Scalar(0,0,255),2);
               /*
                center1.x+=25;center1.y+=40;
                center2.x+=25;center2.y+=40;
                center3.x+=25;center3.y+=40;
                center4.x+=25;center4.y+=40;

                line(cameraFrame, center1, center4, Scalar(0,255,0),2);
                line(cameraFrame, center2, center4, Scalar(0,255,0),2);
                line(cameraFrame, center3, center2, Scalar(0,255,0),2);
                line(cameraFrame, center3, center1, Scalar(0,255,0),2);

                line(cameraFrame, center1, center1_, Scalar(255,0,0),2);
                line(cameraFrame, center2, center2_, Scalar(255,0,0),2);
                line(cameraFrame, center3, center3_, Scalar(255,0,0),2);
                line(cameraFrame, center4, center4_, Scalar(255,0,0),2);
                //* /
            }
            //*/
            //this->usleep(50000);
            //imshow("in",InImage);
            //cv::waitKey(30);
            //imshow("Camera", cameraFrame);
            QImage img = Utils::mat2QImage(cameraFrame);
            emit displayThisImage(img);
            //this->usleep(100);


            cout << "frame rate: "<< 1/(frameRateTime.msecsTo(QTime::currentTime())/1000.0)<<" fps"<<endl;

            frameRateTime = QTime::currentTime();
            camera.read(cameraFrame);
        }
    }
}

