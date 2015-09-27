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

static double angle(Point pt1, Point pt2, Point pt0)
{
double dx1 = pt1.x -pt0.x;
double dx2 = pt2.x - pt0.x;
double dy1 = pt1.y - pt0.y;
double dy2 = pt2.y - pt0.y;
return (dx1 * dx2 + dy1 * dy2)/sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

vector<Rect> detectRect(Mat img)
{
//utilisation du redfilter doit etre utilisée avant l'appel a dRect pour pouvoir autoroute et ville sur la meme fonction
vector<Rect> rects = vector<Rect>();
vector<vector<Point> > contours;
findContours(img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
for (int i = 0; i < contours.size(); i++)
{
vector<Point> approx;
approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
{
double maxcos = 0;
for (int j = 2; j < 5; j++)
{
double cosin = fabs(angle(approx[j%4], approx[j - 2], approx[j- 1]));
maxcos = MAX(maxcos, cosin);
}
if (maxcos < 0.3)
rects.push_back(Rect(approx[0], approx[2]));
//rectangle(img, approx[0], approx[2], Scalar(0,0,0), 1, 8, 0); 
}

}
return rects;
}
