#ifndef RANSAC_HH
#define RANSAC_HH

#include "../filter/filters.hh"

vector<Point> get3Points(OutputArray cnt, int seed);
vector<float> geteq(Point center, vector<Point> point3);
vector<float> checkEllipse(Mat img, vector<vector<Point> >* ret);
vector<float> checkSquare(Mat img, vector<vector<Point> >* ret);
vector<int> checkTriangle(Mat img, vector<vector<Point> >* ret);
#endif /*RANSAC_HH*/
