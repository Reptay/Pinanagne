#ifndef FILTERS_HH
#define FILTERS_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>

using namespace cv;
using namespace std;

void dp(Mat img);
void dp(Mat img, std::string str);
Mat RedFilter(Mat img);
Mat RedFilterSouple(Mat img);
Mat BlueFilter(Mat img);
void BlueRedFilter(Mat img);
Mat BlackFilter(Mat img);
void getContour(Mat& img);
void getOrientation(Mat& img);
Point findcenter(Mat img, vector<vector<Point> >* ret);
int nbWhitePix(Mat img);
Mat onlyBlack(Mat img);
#endif /*FILTERS_HH*/
