#ifndef RECTANGLE_HH
#define RECTANGLE_HH
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>
#include "circle.hh"
using namespace cv;
using namespace std;

class Rectangle
{
public:
  Rectangle(Point2f rect_points[4]);
  void draw(Mat img);
  bool isCircleProportion();
  Circle* getCircle();
private:
  int x_; // point en haut a gauche
  int y_;
  int height_;
  int width_;
};


#endif
