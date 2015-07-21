#include "circle.hh"

Circle::Circle(Point center, float radius){
  this->center_ = center;
  this->radius_ = radius;
}

Point Circle::getCenter(){
  return center_;
}

float Circle::getRadius(){
  return radius_;
}

void Circle::draw(Mat img){

  // circle center
  circle( img, center_, 3, Scalar(0,255,0), -1, 8, 0 );
  
  // circle outline 
  circle( img, center_, radius_, Scalar(0,0,255), 2, 8, 0 );
  
  namedWindow("Display2", WINDOW_AUTOSIZE);
  imshow("Display2", img);
  waitKey(0);
}
