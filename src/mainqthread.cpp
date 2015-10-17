#include "mainqthread.h"
#include "dbscam.h"

using namespace cv;
using namespace std;
using namespace aruco;

MainQThread::MainQThread() {
    srand (time(NULL));
}

vector<CalibPoint> MainQThread::getRandomPixels(Mat &image, int numPoits, Marker A, Marker B, Marker C) {
    vector<CalibPoint>  pixels;

    Point2f origin;
    Point2f vector_i, vector_j;

    origin = A.getCenter();

    vector_i.x = B.getCenter().x - origin.x;
    vector_i.y = B.getCenter().y - origin.y;

    vector_j.x = C.getCenter().x - origin.x;
    vector_j.y = C.getCenter().y - origin.y;



    Mat imageHSV;

    cvtColor(image, imageHSV, CV_BGR2HSV);

    for(int i = 0; i < (numPoits ); i++){
        float alfa = (rand() % 600 + 200)/1000.0;
        float beta = (rand() % 700 + 150)/1000.0;

        Point newPoint;
        newPoint.x = (int) (origin.x + alfa*vector_i.x + beta*vector_j.x);
        newPoint.y = (int) (origin.y + alfa*vector_i.y + beta*vector_j.y);


        Vec3b vecRGB = image.at<cv::Vec3b>(newPoint.y,newPoint.x);
        Vec3b vecHSV = imageHSV.at<cv::Vec3b>(newPoint.y,newPoint.x);


        //circle(image, newPoint,1, Scalar(0,0,255),3);
        //Scalar pix = image.at<Vec3b>(newPoint);

        //cout << "vec = " << vec[2]<< " "<< vec[1]<< " "<< vec[0]<< endl;


        //if (DBSCAM::distancePixels( Scalar(vec[0], vec[1], vec[2]),  Scalar(0,0,0)) < 330){

        pixels.insert(pixels.end(),CalibPoint(Scalar(vecRGB[0], vecRGB[1], vecRGB[2]),Scalar(vecHSV[0], vecHSV[1], vecHSV[2])));

        //}

    }

    return pixels;


}

vector<CalibPoint> MainQThread::getRandomPixels(Mat &image, Mat &grayImage, int numPoits) {

  vector<CalibPoint>  pixels;


  Point2f vector_i, vector_j;


  vector_i.x = image.cols;
  vector_i.y = 0;

  vector_j.x = 0;
  vector_j.y = image.rows;



  Mat imageHSV;

  cvtColor(image, imageHSV, CV_BGR2HSV);

  for(int i = 0; i < (numPoits ); i++){
      float alfa = (rand() % 600 + 200)/1000.0;
      float beta = (rand() % 700 + 150)/1000.0;

      Point newPoint;
      newPoint.x = (int) (0 + alfa*vector_i.x + beta*vector_j.x);
      newPoint.y = (int) (0 + alfa*vector_i.y + beta*vector_j.y);


      Vec3b vecRGB = image.at<cv::Vec3b>(newPoint.y,newPoint.x);
      Vec3b vecHSV = imageHSV.at<cv::Vec3b>(newPoint.y,newPoint.x);


      circle(grayImage, newPoint,1, Scalar(0,0,255),3);
      //Scalar pix = image.at<Vec3b>(newPoint);

      //cout << "vec = " << vec[2]<< " "<< vec[1]<< " "<< vec[0]<< endl;



      if ( vecHSV[2]> 127 && vecHSV[1]>50) {

          pixels.insert(pixels.end(),CalibPoint(Scalar(vecRGB[0], vecRGB[1], vecRGB[2]),Scalar(vecHSV[0], vecHSV[1], vecHSV[2])));
}
     // } else {
     //     i--;
     // }

  }

  return pixels;

}

vector<CalibPoint> MainQThread::getCalibData() const {
  return calibData;
}

void MainQThread::trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed) { // from https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
  int MIN_OBJECT_AREA = 12;
  int MAX_OBJECT_AREA = 40000;
  int MAX_NUM_OBJECTS = 100;

  Mat temp;
  threshold.copyTo(temp);
  //these two vectors needed for output of findContours
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
  //use moments method to find our filtered object
  double refArea = 0;
  bool objectFound = false;
  if (hierarchy.size() > 0) {
      int numObjects = hierarchy.size();
      //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
      if(numObjects<MAX_NUM_OBJECTS){
          for (int index = 0; index < hierarchy.size(); index = hierarchy[index][0]) {

              Moments moment = moments((cv::Mat)contours[index]);
              double area = moment.m00;

              //if the area is less than 20 px by 20px then it is probably just noise
              //if the area is the same as the 3/2 of the image size, probably just a bad filter
              //we only want the object with the largest area so we safe a reference area each
              //iteration and compare it to the area in the next iteration.
              if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                  x = moment.m10/area;
                  y = moment.m01/area;
                  objectFound = true;
                  refArea = area;
                  drawObject(x,y,cameraFeed);
                }else objectFound = false;


            }
          //let user know you found an object
          if(objectFound ==true){
              putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
              //draw object location on screen
              drawObject(x,y,cameraFeed);}

        }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
}

void MainQThread::trackFilteredObject2(int &x, int &y, Mat threshold, Mat &cameraFeed,Scalar c) { // from https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
  int MIN_OBJECT_AREA = 25;
  int MAX_OBJECT_AREA = 40000;
  int MAX_NUM_OBJECTS = 100;

  Scalar color  = Scalar(c[2],c[1],c[0]);

  Mat temp;
  threshold.copyTo(temp);
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE );

  vector<vector<Point> > hull( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
        {  convexHull( Mat(contours[i]), hull[i], false ); }

  //use moments method to find our filtered object
  double refArea = 0;
  bool objectFound = false;
  if (hierarchy.size() > 0) {
      int numObjects = hierarchy.size();
      //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
      if(numObjects<MAX_NUM_OBJECTS){
          for (int index = 0; index < hierarchy.size(); index = hierarchy[index][0]) {

              Moments moment = moments((cv::Mat)contours[index]);
              double area = moment.m00;


              if(area>MIN_OBJECT_AREA ){
              //    if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA ){
                  x = moment.m10/area;
                  y = moment.m01/area;
                  objectFound = true;
                  refArea = area;
                  //drawObject(x,y,cameraFeed);
                  //drawContours( cameraFeed, contours, index, color, CV_FILLED, 8, hierarchy, 0, Point() );
                  drawContours( cameraFeed, hull, index, color, CV_FILLED, 8, hierarchy, 0, Point() );
                }


            }
          if(objectFound ==true){
             // putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
          }

        } else {
          putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
        }
    }
}


void MainQThread::drawObject(int x, int y, Mat &frame) { // from https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
  //use some of the openCV drawing functions to draw crosshairs
  //on your tracked image!

  //UPDATE:JUNE 18TH, 2013
  //added 'if' and 'else' statements to prevent
  //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

  circle(frame,Point(x,y),20,Scalar(0,255,0),2);
  if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
  if(y+25<frame.rows)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(x,frame.rows),Scalar(0,255,0),2);
  if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
  if(x+25<frame.cols)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(frame.cols,y),Scalar(0,255,0),2);

  putText(frame,to_string(x)+","+to_string(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}

void MainQThread::getThresholdSpace() {
  ConfigDAO *config = ConfigDAO::Instance();

  this->minHSV = config->getLastModified().getMinHSV();
  this->maxHSV = config->getLastModified().getMaxHSV();
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

Point MainQThread::pointFromQPoint(QPoint p, double inWidth, double inHeight, double outWidth, double outHeight) {
  Point2f ret;
  ret.x = (int)p.x()*1.0*(outWidth/inWidth);
  ret.y = (int)p.y()*1.0*(outHeight/inHeight);
  return ret;
}

void MainQThread::removeBorders(Mat &src, Mat &out, QPoint topLeft, QPoint bottomLeft, QPoint bottomRight, QPoint topRight, int width, int height) {


  if (topLeft.x()<0 || topRight.x()<0 || bottomLeft.x()<0 || bottomRight.x()<0) {
      out = src.clone();
      cout << "Error whent trying to remove borders of the field" << endl;
      return;
  }







  Point a =pointFromQPoint(topLeft, 640, 480, src.cols, src.rows);
  Point b =pointFromQPoint(bottomLeft, 640, 480, src.cols, src.rows);
  Point c =pointFromQPoint(bottomRight, 640, 480, src.cols, src.rows);
  Point d =pointFromQPoint(topRight, 640, 480, src.cols, src.rows);

    //http://stackoverflow.com/questions/7838487/executing-cvwarpperspective-for-a-fake-deskewing-on-a-set-of-cvpoint
    // and http://stackoverflow.com/questions/14179164/opencv-image-transformation-and-perspective-change


   /* vector<Point> not_a_rect_shape;
    not_a_rect_shape.push_back(a);
    not_a_rect_shape.push_back(b);
    not_a_rect_shape.push_back(c);
    not_a_rect_shape.push_back(d);

    // For debugging purposes, draw green lines connecting those points
    // and save it on disk
//    /*
    const Point* point = &not_a_rect_shape[0];
    int n = (int)not_a_rect_shape.size();
    Mat draw = src.clone();
    polylines(draw, &point, &n, 1, true, Scalar(0, 255, 0), 3, CV_AA);
    imwrite("draw.jpg", draw);
*/

     Point2f src_vertices[4];
    src_vertices[0] = a;
    src_vertices[1] = b;
    src_vertices[2] = c;
    src_vertices[3] = d;

    Point2f dst_vertices[4];
    dst_vertices[0] = Point(0, 0);
    dst_vertices[1] = Point(0, height-1);
    dst_vertices[2] = Point(width-1, height-1);
    dst_vertices[3] = Point(width-1, 0);

    Mat warpPerspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);

    cv::Mat rotated;
    cv::Size size(width, height);

    //warpAffine(src, rotated, warpAffineMatrix, size, INTER_LINEAR, BORDER_CONSTANT);
    warpPerspective(src, rotated, warpPerspectiveMatrix, size);


    //imwrite("draw.jpg", rotated);
    out = rotated.clone();
//*/
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
    Mat cameraFrameRow;
    Mat cameraFrame;
    Mat imageHSV;
    MarkerDetector MDetector;
    vector<Marker> Markers;

    Marker m50, m51, m52, m53;

    Point2f center1;
    Point2f center2;
    Point2f center3;
    Point2f center4;
    Mat subRectangle;
    Mat subRectangleGray;
    Mat subRectangleSobel;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

     Mat erodeElement;
     Mat dilateElement;


     ConfigDAO *config = ConfigDAO::Instance();




    if (!this->isInterruptionRequested()){


        camera.read(cameraFrameRow);

        while(!this->isInterruptionRequested()){

            this->minHSV = config->getLastModified().getMinHSV();
            this->maxHSV = config->getLastModified().getMaxHSV();
            this->removeBorders(cameraFrameRow, cameraFrame, config->getCornerLeftTop(), config->getCornerLeftBottom(), config->getCornerRightBottom(), config->getCornerRightTop(), 640,480);





            cvtColor(cameraFrame, imageHSV, CV_BGR2HSV);
            //*


/*
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


*/


            if (false) {
            //if (Markers.size()>=4){

                for (unsigned int i=0;i<Markers.size();i++) {
                    if (Markers[i].id==50){
                        m50 = Markers[i];
                    }
                    if (Markers[i].id==51){
                        m51 = Markers[i];
                    }
                    if (Markers[i].id==52){
                        m52 = Markers[i];
                    }
                    if (Markers[i].id==53){
                        m53 = Markers[i];
                    }
                }

                /*
                vector<CalibPoint> v = getRandomPixels(cameraFrame,50,m50,m52,m53);
                calibData.clear();
                calibData.insert(calibData.end(), v.begin(), v.end());
                emit sendCalibData();*/




                center1 =  m50.getCenter();
                center2 =  m53.getCenter();
                center3 =  m51.getCenter();
                center4 =  m52.getCenter();


                getSubTrapezoid(cameraFrame,subRectangle,center1, center2, center3, center4);
                //getSubTrapezoid(cameraFrame,subRectangle,Markers[0].getCenter(), Markers[1].getCenter(), Markers[2].getCenter(), Markers[3].getCenter());


                blur( subRectangle, subRectangle, Size(3,3));
                cvtColor(subRectangle, subRectangleGray, CV_BGR2GRAY);

                /// Generate grad_x and grad_y

                int scale = 1;
                int delta = 0;
                int ddepth = CV_16S;



                /// Gradient X
                Scharr( subRectangleGray, grad_x, ddepth, 1, 0,  scale, delta, BORDER_DEFAULT );
                //Sobel( subRectangleGray, grad_x, ddepth, 1, 0, 5, scale, delta, BORDER_DEFAULT );
                convertScaleAbs( grad_x, abs_grad_x );

                /// Gradient Y
                Scharr( subRectangleGray, grad_y, ddepth, 0, 1,  scale, delta, BORDER_DEFAULT );
                //Sobel( subRectangleGray, grad_y, ddepth, 0, 1, 5, scale, delta, BORDER_DEFAULT );
                convertScaleAbs( grad_y, abs_grad_y );

                /// Total Gradient (approximate)
                addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, subRectangleSobel );

                inRange(subRectangleSobel,cv::Scalar(45, 0, 0),cv::Scalar(255, 255, 255), subRectangleSobel);

                medianBlur(subRectangleSobel, subRectangleSobel, 5);

                erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
                //dilate with larger element so make sure object is nicely visible
                dilateElement = getStructuringElement( MORPH_RECT,Size(3,3));



//                erode(subRectangleSobel,subRectangleSobel,erodeElement);

                //dilate(subRectangleSobel,subRectangleSobel,dilateElement);

                //erode(subRectangleSobel,subRectangleSobel,erodeElement);
                //vector<CalibPoint> v = getRandomPixels(cameraFrame,subRectangleSobel,500);
               // calibData.clear();
               // calibData.insert(calibData.end(), v.begin(), v.end());
                //emit sendCalibData();

                QImage imgREC = Utils::mat2QImage(subRectangleSobel);
                emit displayThisImageMin(imgREC);


                Point2f center1_ =  Markers[0].getCenter();
                Point2f center2_ =  Markers[1].getCenter();
                Point2f center3_ =  Markers[2].getCenter();
                Point2f center4_ =  Markers[3].getCenter();

                line(cameraFrame, center1, center2, Scalar(0,0,255),2);
                line(cameraFrame, center2, center3, Scalar(0,0,255),2);
                line(cameraFrame, center3, center4, Scalar(0,0,255),2);
                line(cameraFrame, center4, center1, Scalar(0,0,255),2);
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



            Mat thresh;

            vector<ColorSpace> colorSpaces = config->getColorSpaces();
            vector<ColorSpace>::iterator it = colorSpaces.begin();
            ColorSpace cs;


            for (; it!= colorSpaces.end(); it++) {
                cs = *(it);
                inRange(imageHSV,cs.getMinHSV(),cs.getMaxHSV(),thresh);

                    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
                //dilate with larger element so make sure object is nicely visible
                    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));

                    erode(thresh,thresh,erodeElement);
                    //erode(thresh,thresh,erodeElement);
                    //dilate(thresh,thresh,erodeElement);
                    //dilate(thresh,thresh,erodeElement);
                    dilate(thresh,thresh,erodeElement);
                    int x = 0, y = 0;
                    trackFilteredObject2(x,y,thresh,cameraFrame, cs.getColor());





            }




                inRange(imageHSV,this->minHSV,this->maxHSV,thresh);

                    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
                //dilate with larger element so make sure object is nicely visible
                    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));

                    erode(thresh,thresh,erodeElement);
                    //erode(thresh,thresh,erodeElement);
                    //dilate(thresh,thresh,erodeElement);
                    //dilate(thresh,thresh,erodeElement);
                    dilate(thresh,thresh,erodeElement);


            QImage imgs = Utils::mat2QImage(thresh);
            emit displayThrashold(imgs);


            //QImage imgss = Utils::mat2QImage(cameraFrame);
            //emit displayThisImageMin(imgss);

            QImage imgsss = Utils::mat2QImage(cameraFrameRow);
            emit displayImageSelectBorders(imgsss);

            QImage img = Utils::mat2QImage(cameraFrame);
            emit displayThisImage(img);
            //this->usleep(100);


            //cout << "frame rate: "<< 1/(frameRateTime.msecsTo(QTime::currentTime())/1000.0)<<" fps"<<endl;

            //frameRateTime = QTime::currentTime();
            camera.read(cameraFrameRow);
        }
    }
}
