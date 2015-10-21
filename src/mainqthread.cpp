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

vector<Point>  MainQThread::trackFilteredObject2( Mat threshold, Mat &cameraFeed, Scalar c) { // from https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
  int MIN_OBJECT_AREA = 25;
  int MAX_OBJECT_AREA = 400;
  int MAX_NUM_OBJECTS = 20;
  int x,y;

  Scalar color  = Scalar(c[2],c[1],c[0]);

  Mat temp;
  threshold.copyTo(temp);
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE );

  temp.release();


  vector<Point> positions;

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


              if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA ){
                  x = moment.m10/area;
                  y = moment.m01/area;

                  positions.push_back(Point(x,y));
                  objectFound = true;
                  refArea = area;
                  drawContours( cameraFeed, hull, index, color, CV_FILLED, 8, hierarchy, 0, Point() );

                 // drawObject(x,y,cameraFeed);
                  //drawContours( cameraFeed, contours, index, color, CV_FILLED, 8, hierarchy, 0, Point() );
                }


            }
          if(objectFound ==true){
             // putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
          }

        } else {
          putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
        }
    }
  return positions;
}

vector<Point>  MainQThread::trackFilteredObject2Ball( Mat threshold, Mat &cameraFeed, Scalar c) { // from https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
  int MIN_OBJECT_AREA = 25;
  int MAX_OBJECT_AREA = 400;
  int MAX_NUM_OBJECTS = 20;
  int x,y;

  Scalar color  = Scalar(c[2],c[1],c[0]);

  Mat temp;
  threshold.copyTo(temp);
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE );

  temp.release();


  vector<Point> positions;

  vector<vector<Point> > hull( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );
  vector<vector<Point> > contours_poly( contours.size() );


  for( int i = 0; i< contours.size(); i++ ) {
      approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
      convexHull( Mat(contours[i]), hull[i], false );
      minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }

  //use moments method to find our filtered object
  int numObjects = hierarchy.size();
  //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
  if(numObjects<MAX_NUM_OBJECTS){
      for( int index = 0; index< contours.size(); index++ ) {

          Moments moment = moments((cv::Mat)contours[index]);
          double area = moment.m00;


          if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && radius[index] <= 6 && radius[index] > 2){
          //if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA ){
              x = moment.m10/area;
              y = moment.m01/area;

              positions.push_back(Point(x,y));

              drawContours( cameraFeed, hull, index, color, CV_FILLED, 8, hierarchy, 0, Point() );
            }
        }

    } else {
      putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }

  return positions;
}

void MainQThread::drawObject(int x, int y, Mat &frame) { // from https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp
  //use some of the openCV drawing functions to draw crosshairs
  //on your tracked image!

  //UPDATE:JUNE 18TH, 2013
  //added 'if' and 'else' statements to prevent
  //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

  int size = 10;

  //circle(frame,Point(x,y),size,Scalar(0,255,0),2);
  if(y-size>0)
    line(frame,Point(x,y),Point(x,y-size),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
  if(y+size<frame.rows)
    line(frame,Point(x,y),Point(x,y+size),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(x,frame.rows),Scalar(0,255,0),2);
  if(x-size>0)
    line(frame,Point(x,y),Point(x-size,y),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
  if(x+size<frame.cols)
    line(frame,Point(x,y),Point(x+size,y),Scalar(0,255,0),2);
  else line(frame,Point(x,y),Point(frame.cols,y),Scalar(0,255,0),2);

  //putText(frame,to_string(x)+","+to_string(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

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



  ConfigDAO *config = ConfigDAO::Instance();


  int border = config->getFieldBorder();
  int goalDepth = config->getFieldGoalDepth();


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
    dst_vertices[0] = Point(border+goalDepth, border);
    dst_vertices[1] = Point(border+goalDepth, height-1-border);
    dst_vertices[2] = Point(width-1-(border+goalDepth), height-1- border);
    dst_vertices[3] = Point(width-1-(border+goalDepth), border);

    Mat warpPerspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);

    cv::Mat rotated;
    cv::Size size(width, height);

    //warpAffine(src, rotated, warpAffineMatrix, size, INTER_LINEAR, BORDER_CONSTANT);
    warpPerspective(src, rotated, warpPerspectiveMatrix, size);

    warpPerspectiveMatrix.release();
    //imwrite("draw.jpg", rotated);
    out = rotated.clone();
//*/
}


void MainQThread::cmToPixel(Mat &img, double x, double y, int &px, int &py) {

  ConfigDAO *config = ConfigDAO::Instance();

  px = round(config->getFieldBorder() + config->getFieldGoalDepth() + ((double)(img.cols - 2*config->getFieldBorder() - 2*config->getFieldGoalDepth()))/((double)(config->getFieldWidth())) * x);
  py = round(config->getFieldBorder() +                               ((double)(img.rows - 2*config->getFieldBorder()                              ))/((double)(config->getFieldHeight())) * y);
}

void MainQThread::cmToPixelProportion(Mat &img, double &px, double &py) {

  ConfigDAO *config = ConfigDAO::Instance();

  px = ((double)(img.cols - 2*config->getFieldBorder() - 2*config->getFieldGoalDepth()))/((double)(config->getFieldWidth()));
  py = ((double)(img.rows - 2*config->getFieldBorder()                                ))/((double)(config->getFieldHeight()));

}

Point2d MainQThread::cmToPixelProportionPoint(Mat &img) {
  double px,py;
  ConfigDAO *config = ConfigDAO::Instance();

  px = ((double)(img.cols - 2*config->getFieldBorder() - 2*config->getFieldGoalDepth()))/((double)(config->getFieldWidth()));
  py = ((double)(img.rows - 2*config->getFieldBorder()                                ))/((double)(config->getFieldHeight()));
  return Point2d(px,py);
}

Point2d MainQThread::pixelToCm(Point2d proportion, Point2d pixelPos) {
  ConfigDAO *config = ConfigDAO::Instance();
  double x, y;
  x = (pixelPos.x - config->getFieldGoalDepth() - config->getFieldBorder())/proportion.x - config->getFieldWidth()/2.0;
  y = - (pixelPos.y - config->getFieldBorder())/proportion.y + config->getFieldHeight()/2.0;

  return Point2d(x,y);
}

Point2d MainQThread::cmToPixelPoint(Point2d proportion, Point2d cmPos) {
  ConfigDAO *config = ConfigDAO::Instance();
  double x, y;
  x = (cmPos.x + config->getFieldWidth()/2.0)* proportion.x  + config->getFieldGoalDepth() + config->getFieldBorder();
  y = ( - cmPos.y + config->getFieldHeight()/2.0)* proportion.y  + config->getFieldBorder();

  return Point2d(x,y);
}

double MainQThread::euclidianDistance(Point2d a, Point2d b) {
    return sqrt((a.x-b.x)*(a.x-b.x)+
                (a.y-b.y)*(a.y-b.y));
}

QColor MainQThread::scalarToQColor(Scalar c) {
  return QColor(c[2],c[1],c[0]);
}

Scalar MainQThread::qColorToScalar(QColor c) {
  return Scalar(c.red(),c.green(),c.blue());
}

void MainQThread::drawField(Mat &image, Scalar color) {

  //color = Scalar(0,255,0);

  ConfigDAO *config = ConfigDAO::Instance();

  int width = image.cols;
  int height = image.rows;

  int fieldBorder = config->getFieldBorder();
  int fieldWidth  = config->getFieldWidth();
  int fieldDepth  = config->getFieldGoalDepth();

  line(image, Point(fieldDepth+fieldBorder, fieldBorder), Point(fieldDepth+fieldBorder, height - fieldBorder),color,2);
  line(image, Point(fieldDepth+fieldBorder, height - fieldBorder), Point(width - fieldDepth - fieldBorder - 1, height - fieldBorder),color,2);
  line(image, Point(width - fieldDepth - fieldBorder - 1, fieldBorder), Point(width - fieldDepth - fieldBorder - 1, height - fieldBorder),color,2);
  line(image, Point(width - fieldDepth - fieldBorder - 1, fieldBorder), Point(fieldDepth+fieldBorder, fieldBorder),color,2);

  line(image, Point(width/2, fieldBorder), Point(width/2, height - fieldBorder),color,2);


  int x,y;
  double px,py;
  cmToPixel(image,config->getFieldWidth()/2.0, config->getFieldHeight()/2.0, x, y);
  cmToPixelProportion(image, px, py);
  ellipse(image,Point(x,y),Size(px*20, py*20), 360,0,360,color,2);
  //cout << px << ", " << py << endl;



}

void MainQThread::drawRobot(Mat &image, Robot robot, Point2d proportion) {
  Point2d pixPos = cmToPixelPoint(proportion, robot.getPosition());
  Scalar color = qColorToScalar(robot.getColorRobot());
  Scalar team = qColorToScalar(robot.getColorTeam());

  circle(image, pixPos, 20, team, 2);
  circle(image, pixPos, 15, color, 2);
}


void MainQThread::run() {

  Camera *camera = Camera::getCamera();
  CameraParameters cp;
  QTime frameRateTime;
  frameRateTime= QTime::currentTime();

  if (!camera->isOpened()) { //check if video device has been initialised
      cout << "cannot open camera";
      return;
    }
  Mat cameraFrameRow;
  Mat cameraFrame;
  Mat imageHSV;

  Mat elementMorph3;
  Mat elementMorph5;

  Mat thresh;
  vector<ColorSpace> colorSpaces;
  vector<ColorSpace>::iterator it;

  elementMorph3 = getStructuringElement( MORPH_RECT,Size(3,3));
  elementMorph5 = getStructuringElement( MORPH_RECT,Size(5,5));



  ConfigDAO *config = ConfigDAO::Instance();

  vector<Point> blueObjects;
  vector<Point> yellowObjects;
  vector<Point> orangeObjects;
  vector<Point> greenObjects;
  vector<Point> purpleObjects;

  ColorSpace yellow;
  ColorSpace blue;
  ColorSpace green;
  ColorSpace purple;

  blueObjects.clear();
  yellowObjects.clear();
  orangeObjects.clear();
  greenObjects.clear();
  purpleObjects.clear();

  double minDist = 2;
  double maxDist = 10;

  vector<Robot> robots;
  robots.clear();

  if (!this->isInterruptionRequested()){


      camera->read(cameraFrameRow);

      while(!this->isInterruptionRequested()){

          cout << "frame rate: "<< (frameRateTime.msecsTo(QTime::currentTime()))<<" - ";

          this->minHSV = config->getLastModified().getMinHSV();
          this->maxHSV = config->getLastModified().getMaxHSV();
          this->removeBorders(cameraFrameRow, cameraFrame, config->getCornerLeftTop(), config->getCornerLeftBottom(), config->getCornerRightBottom(), config->getCornerRightTop(), 400,300);

          cvtColor(cameraFrame, imageHSV, CV_BGR2HSV);

          colorSpaces = config->getColorSpaces();
          it = colorSpaces.begin();
          ColorSpace cs;

          Mat imageBlack( cameraFrame.rows, cameraFrame.cols, CV_8UC3, Scalar(0,0,0));
          drawField(imageBlack, Scalar(255,255,255));


          for (; it!= colorSpaces.end(); it++) {
              cs = *(it);

              if (cs.getName().compare("yellow")==0) {
                  yellow = cs;
                  inRange(imageHSV,cs.getMinHSV(),cs.getMaxHSV(),thresh);
                  erode(thresh,thresh,elementMorph3);
                  dilate(thresh,thresh,elementMorph3);
                  yellowObjects = trackFilteredObject2(thresh,imageBlack, cs.getColor());
                }

              if (cs.getName().compare("blue")==0) {
                  blue = cs;
                  inRange(imageHSV,cs.getMinHSV(),cs.getMaxHSV(),thresh);
                  erode(thresh,thresh,elementMorph3);
                  dilate(thresh,thresh,elementMorph3);
                  blueObjects = trackFilteredObject2(thresh,imageBlack, cs.getColor());
                }

              if (cs.getName().compare("orange")==0) {
                  inRange(imageHSV,cs.getMinHSV(),cs.getMaxHSV(),thresh);
                  erode(thresh,thresh,elementMorph3);
                  //erode(thresh,thresh,elementMorph3);
                  //dilate(thresh,thresh,elementMorph3);
                  dilate(thresh,thresh,elementMorph3);
                  orangeObjects = trackFilteredObject2Ball(thresh,imageBlack, cs.getColor());
                }

              if (cs.getName().compare("green")==0) {
                  green = cs;
                  inRange(imageHSV,cs.getMinHSV(),cs.getMaxHSV(),thresh);
                  erode(thresh,thresh,elementMorph3);
                  dilate(thresh,thresh,elementMorph3);
                  greenObjects = trackFilteredObject2(thresh,imageBlack, cs.getColor());
                }

              if (cs.getName().compare("purple")==0) {
                  purple = cs;
                  inRange(imageHSV,cs.getMinHSV(),cs.getMaxHSV(),thresh);
                  erode(thresh,thresh,elementMorph3);
                  dilate(thresh,thresh,elementMorph3);
                  purpleObjects = trackFilteredObject2(thresh,imageBlack, cs.getColor());
                }
            }

          Point2d proportion;
          proportion = cmToPixelProportionPoint(cameraFrame);

          if (orangeObjects.size() == 0) {
              putText(cameraFrame,"No Ball",Point(0,50),2,1,Scalar(0,0,255),2);
          } else if (orangeObjects.size() >1) {
              putText(cameraFrame,"Too many balls",Point(0,50),2,1,Scalar(0,0,255),2);
          } else {
              //Point ballAt = orangeObjects.at(0);
              //cout << "ball X: " << (ballAt.x - config->getFieldGoalDepth() - config->getFieldBorder())/px << endl;
              //cout << "ball Y: " << (ballAt.y - config->getFieldBorder())/py << endl;
            }


          //Corrigir depois



          robots.clear();

          for (int i = 0; i<yellowObjects.size(); i++) {
              bool found = false;
              Point2d min;
              Point2d basePointCm = pixelToCm(proportion, yellowObjects[i]);
              Point2d calc, other;
              double otherDist;
              QColor color;


              for (int j = 0; j<greenObjects.size(); j++) {
                  calc = pixelToCm(proportion, greenObjects[j]);
                  double dist = euclidianDistance(basePointCm, calc);

                  if (dist >= minDist && dist <=maxDist && (!found || dist < otherDist)) {
                      found = true;
                      otherDist = dist;
                      other = calc;
                      color = scalarToQColor(green.getColor());
                    }
                }

              for (int j = 0; j<purpleObjects.size(); j++) {
                  calc = pixelToCm(proportion, purpleObjects[j]);
                  double dist = euclidianDistance(basePointCm, calc);

                  if (dist >= minDist && dist <=maxDist && (!found || dist < otherDist)) {
                      found = true;
                      otherDist = dist;
                      other = calc;
                      color = scalarToQColor(purple.getColor());
                    }
                }

              if (found) {
                  Point2d robotPoint(basePointCm.x,basePointCm.y);
                  double ang = atan2(other.y - basePointCm.y,other.x - basePointCm.x);
                  Robot r;
                  r.setAngle(ang);
                  r.setColorRobot(color);
                  r.setColorTeam(scalarToQColor(yellow.getColor()));
                  r.setPosition(robotPoint);
                  robots.push_back(r);
                }

            }

          for (int i = 0; i<blueObjects.size(); i++) {
              bool found = false;
              Point2d min;
              Point2d basePointCm = pixelToCm(proportion, blueObjects[i]);
              Point2d calc, other;
              double otherDist;
              QColor color;


              for (int j = 0; j<greenObjects.size(); j++) {
                  calc = pixelToCm(proportion, greenObjects[j]);
                  double dist = euclidianDistance(basePointCm, calc);

                  if (dist >= minDist && dist <=maxDist && (!found || dist < otherDist)) {
                      found = true;
                      otherDist = dist;
                      other = calc;
                      color = scalarToQColor(green.getColor());
                    }
                }

              for (int j = 0; j<purpleObjects.size(); j++) {
                  calc = pixelToCm(proportion, purpleObjects[j]);
                  double dist = euclidianDistance(basePointCm, calc);

                  if (dist >= minDist && dist <=maxDist && (!found || dist < otherDist)) {
                      found = true;
                      otherDist = dist;
                      other = calc;
                      color = scalarToQColor(purple.getColor());
                    }
                }

              if (found) {
                  Point robotPoint(basePointCm.x,basePointCm.y);
                  double ang = atan2(other.y - basePointCm.y,other.x - basePointCm.x);
                  Robot r;
                  r.setAngle(ang);
                  r.setColorRobot(color);
                  r.setColorTeam(scalarToQColor(blue.getColor()));
                  r.setPosition(robotPoint);
                  robots.push_back(r);
                }

            }

          for (int i = 0; i<robots.size(); i++) {
              drawRobot(imageBlack, robots[i], proportion);
            }










          inRange(imageHSV,this->minHSV,this->maxHSV,thresh);
          erode(thresh,thresh,elementMorph3);
          dilate(thresh,thresh,elementMorph3);


          cout << (frameRateTime.msecsTo(QTime::currentTime()))<<" - ";

          QImage imgs = Utils::mat2QImage(thresh);
          emit displayThrashold(imgs);


          QImage imgss = Utils::mat2QImage(cameraFrame);
          emit displayThisImageMin(imgss);

          QImage imgsss = Utils::mat2QImage(cameraFrameRow);
          emit displayImageSelectBorders(imgsss);

          QImage img = Utils::mat2QImage(imageBlack);
          emit displayThisImage(img);
          //this->usleep(100);


          cout << (frameRateTime.msecsTo(QTime::currentTime()))<<" msecs"<<endl;

          frameRateTime = QTime::currentTime();
          camera->read(cameraFrameRow);
          //this->usleep(100000);
        }
    }

  cameraFrameRow.release();
  cameraFrame.release();
  imageHSV.release();
  elementMorph3.release();
  elementMorph5.release();
  thresh.release();
}


void MainQThread::markers() {
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


* /


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
      emit sendCalibData();* /




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
      //* /
  }
  //*/
  //this->usleep(50000);
  //imshow("in",InImage);
  //cv::waitKey(30);
  //imshow("Camera", cameraFrame);

}
