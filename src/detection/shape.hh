#ifndef SHAPE_HH
#define SHAPE_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>
#include "circle.hh"
using namespace cv;
using namespace std;

Circle* getCircle(Mat img);

#endif