#ifndef CIRCLE_HH
#define CIRCLE_HH
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>
using namespace cv;
using namespace std;

class Circle
{
public:
  Circle(Point center, float radius);
  void draw(Mat img);
  Point getCenter();
  float getRadius();
private:
  Point center_;
  float radius_;
};

#endif
