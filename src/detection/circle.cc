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
