#ifndef FILTERS_HH
#define FILTERS_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void RedFilter(Mat img);
void BlueFilter(Mat img);
void BlueRedFilter(Mat img);

#endif /*FILTERS_HH*/
