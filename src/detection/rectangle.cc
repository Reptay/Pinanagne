#include "rectangle.hh"

double comparerc(int a, int b)
{
  double i = a;
  double j = b;
  if (i > j)
    return i / j;
  else
    return j / i;
}
int max(int a, int b)
{
  if (a>b)
    return a;
  else
    return b;
}

Rectangle::Rectangle(Point2f rect_points[4]){
  int minx = (int) rect_points[0].x;
  int maxx = minx;
  int miny = (int) rect_points[0].y;
  int maxy = miny;

  for (int i = 1; i < 4; i++){
    if (rect_points[i].x < minx)
      minx = rect_points[i].x;
    else if (rect_points[i].x > maxx)
      maxx = rect_points[i].x;
    if (rect_points[i].y < miny)
      miny = rect_points[i].y;
    else if (rect_points[i].y>maxy)
      maxy=rect_points[i].y;
  }
  x_ = minx;
  y_ = miny;
  width_ = maxx-minx;
  height_ = maxy-miny;
}

void Rectangle::draw(Mat img)
{
  Scalar color = Scalar(0, 0, 255);
  int ep = 1; //epaisseur de la ligne
  line (img, Point(x_,y_), Point(x_+width_, y_), color, ep, 8);
  line (img, Point(x_+width_,y_), Point(x_+width_, y_+height_), color, ep, 8);
  line (img, Point(x_+width_,y_+height_), Point(x_, y_+height_), color, ep, 8);
  line (img, Point(x_,y_+height_), Point(x_, y_), color, ep, 8); 
}

bool Rectangle::isCircleProportion()
{
  if (comparerc(width_, height_) < 1.02 &&
      width_ > 20)
    return true;
  else
    return false;
}
Circle* Rectangle::getCircle(){
  int cx = x_ + ((int) width_/2);
  int cy = y_+ ((int) height_/2);
  int coteMax = max(width_, height_);
  int radius = (int) coteMax/2;
  return new Circle(Point(cx,cy),radius);
}
