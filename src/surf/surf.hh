#ifndef SURF_HH
#define SURF_HH

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

int findObject(Mat sceneP, Mat objectP, int minHessian, Scalar color, Mat Outimg);

#endif
