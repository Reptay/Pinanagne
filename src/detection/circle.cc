#include "circle.hh"

double distEucl(Point a, Point b)
{
  return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

Circle::Circle(Point center, float radius){
  this->center_ = center;
  this->radius_ = radius;
}

Circle::Circle(Point center, float radius, double factorSize){
  this->center_ = center;
  this->radius_ = radius;
  this->center_.x /= factorSize;
  this->center_.y /= factorSize;
  this->radius_ /= factorSize;
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
}

void Circle::draw(Mat img, int r, int g, int b){
  // circle center
  circle( img, center_, 3, Scalar(0,255,0), -1, 8, 0 );

  // circle outline
  circle( img, center_, radius_, Scalar(r,g,b), 2, 8, 0 );
}

bool Circle::isInside(Circle* c)
{
  double distCtr = distEucl(center_, c->getCenter());
  if (distCtr + radius_ < c->getRadius())
    return true;
  else
    return false;
}
