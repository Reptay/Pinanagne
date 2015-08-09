#ifndef FILTERS_HH
#define FILTERS_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>


using namespace cv;
using namespace std;

void RedFilter(Mat img);
void BlueFilter(Mat img);
void BlueRedFilter(Mat img);
void BlackFilter(Mat img);
void getContour(Mat& img);
void getOrientation(Mat& img);
Point findcenter(Mat img, vector<vector<Point> >* ret);
#endif /*FILTERS_HH*/
