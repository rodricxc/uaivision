#include "mainqthread.h"
#include "dbscam.h"

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



    //Mat imageHSV;

    //cvtColor(image, imageHSV, CV_BGR2HSV);

    for(int i = 0; i < (numPoits ); i++){
        float alfa = (rand() % 250 + 200)/1000.0;
        float beta = (rand() % 700 + 150)/1000.0;

        Point newPoint;
        newPoint.x = (int) (origin.x + alfa*vector_i.x + beta*vector_j.x);
        newPoint.y = (int) (origin.y + alfa*vector_i.y + beta*vector_j.y);


        Vec3b vec = image.at<cv::Vec3b>(newPoint.y,newPoint.x);


        //circle(image, newPoint,1, Scalar(0,0,255),3);
        //Scalar pix = image.at<Vec3b>(newPoint);

        //cout << "vec = " << vec[2]<< " "<< vec[1]<< " "<< vec[0]<< endl;


        if (DBSCAM::distancePixels( Scalar(vec[0], vec[1], vec[2]),  Scalar(0,0,0)) < 300){

            pixels.insert(pixels.end(), Scalar(vec[0], vec[1], vec[2]));

        }

    }

    return pixels;


}
vector<Scalar> MainQThread::getCalibData() const
{
    return calibData;
}

void MainQThread::getSubTrapezoid(Mat &src, Mat &out, Point a, Point b, Point c, Point d) {

    //http://stackoverflow.com/questions/7838487/executing-cvwarpperspective-for-a-fake-deskewing-on-a-set-of-cvpoint

    vector<Point> not_a_rect_shape;
    not_a_rect_shape.push_back(a);
    not_a_rect_shape.push_back(b);
    not_a_rect_shape.push_back(c);
    not_a_rect_shape.push_back(d);

    // For debugging purposes, draw green lines connecting those points
    // and save it on disk
    /*
    const Point* point = &not_a_rect_shape[0];
    int n = (int)not_a_rect_shape.size();
    Mat draw = src.clone();
    polylines(draw, &point, &n, 1, true, Scalar(0, 255, 0), 3, CV_AA);
    imwrite("draw.jpg", draw);//*/

    // Assemble a rotated rectangle out of that info
    RotatedRect box = minAreaRect(cv::Mat(not_a_rect_shape));
    //std::cout << "Rotated box set to (" << box.boundingRect().x << "," << box.boundingRect().y << ") " << box.size.width << "x" << box.size.height << std::endl;

    Point2f pts[4];

    box.points(pts);

    cv::Point2f src_vertices[3];
    src_vertices[0] = pts[0];
    src_vertices[1] = pts[1];
    src_vertices[2] = pts[3];
    //src_vertices[3] = not_a_rect_shape[3];

    Point2f dst_vertices[3];
    dst_vertices[0] = Point(0, 0);
    dst_vertices[1] = Point(box.boundingRect().width-1, 0);
    dst_vertices[2] = Point(0, box.boundingRect().height-1);

    Mat warpAffineMatrix = getAffineTransform(src_vertices, dst_vertices);

    cv::Mat rotated;
    cv::Size size(box.boundingRect().width, box.boundingRect().height);
    warpAffine(src, rotated, warpAffineMatrix, size, INTER_LINEAR, BORDER_CONSTANT);

    out = rotated.clone();

}


void MainQThread::run() {

    Camera camera = Camera::getCamera();
    CameraParameters cp;
    //QTime frameRateTime;
    //frameRateTime= QTime::currentTime();

    if (!camera.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
        return;
    }

    if (!this->isInterruptionRequested()){

        Mat cameraFrame;
        camera.read(cameraFrame);

        while(!this->isInterruptionRequested()){
            Mat imageHSV;

            cvtColor(cameraFrame, imageHSV, CV_BGR2HSV);
            //*
            MarkerDetector MDetector;
            vector<Marker> Markers;

            Marker A, B, C;


            //Ok, let's detect
            MDetector.detect(cameraFrame,Markers);
            //for each marker, draw info and its boundaries in the image
            for (unsigned int i=0;i<Markers.size();i++) {
                //  cout<<Markers[i]<<endl;

                //Markers[i].draw(cameraFrame,Scalar(0,0,255),2);
                if (cp.isValid()){
                    // aruco::CvDrawingUtils::draw3dCube(cameraFrame,Markers[i],cp);
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


                vector<Scalar> v = getRandomPixels(cameraFrame,50,A,B,C);
                calibData.clear();
                calibData.insert(calibData.end(), v.begin(), v.end());
                emit sendCalibData();




                Point2f center1 =  Markers[0].getCenter();
                Point2f center2 =  Markers[1].getCenter();
                Point2f center3 =  Markers[2].getCenter();
                Point2f center4 =  Markers[3].getCenter();

                Mat subRectangle;
                getSubTrapezoid(cameraFrame,subRectangle,center1, center2, center3, center4);

                QImage imgREC = Utils::mat2QImage(subRectangle);
                emit displayThisImageMin(imgREC);


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
                //*/
            }
            //*/
            //this->usleep(50000);
            //imshow("in",InImage);
            //cv::waitKey(30);
            //imshow("Camera", cameraFrame);
            QImage img = Utils::mat2QImage(cameraFrame);
            emit displayThisImage(img);

            //QImage imgHSV = Utils::mat2QImage(imageHSV);
            //emit displayThisImageMin(imgHSV);
            //this->usleep(100);


            //cout << "frame rate: "<< 1/(frameRateTime.msecsTo(QTime::currentTime())/1000.0)<<" fps"<<endl;

            //frameRateTime = QTime::currentTime();
            camera.read(cameraFrame);
        }
    }
}

