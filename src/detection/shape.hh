#ifndef SHAPE_HH
#define SHAPE_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include "circle.hh"
#include "../filter/filters.hh"
#include "rectangle.hh"

using namespace cv;
using namespace std;

/**
 * Detecte tous les ronds dans l'images
 */
std::vector<Circle*> getCircles(Mat img);

std::vector<Circle*> getCirclesByEllipses(Mat src);
#endif
