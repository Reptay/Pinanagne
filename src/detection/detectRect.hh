#ifndef DETECTRECT_HH
#define DETECTRECT_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>
#include "../filter/filters.hh"
#include "../surf/surf.hh"
using namespace cv;
using namespace std;

vector<Rect> detectRect(Mat img); 

bool isCity(Mat img);
bool isHighWay(Mat img);

#endif
