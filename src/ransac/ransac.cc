#include "ransac.hh"

vector<Point> get3Points(vector<Point> cnt, int seed)
{
	srand(seed);
	vector<Point> point3 = vector<Point>();
	while (point3.size() < 3)
	{
		int x = rand() % cnt.size();
		point3.push_back(cnt[x]);
	}
	return point3;	
}

vector<float> geteq(Point center, vector<Point> point3)
{
	int xc = center.x;
	int yc = center.y;
	int x1 = point3[0].x - xc;
	int y1 = point3[0].y - yc;
	int x2 = point3[1].x - xc;
	int y2 = point3[1].y - yc;
	int x3 = point3[2].x - xc;
	int y3 = point3[2].y - yc;
	float divc = (y3^2) - ((y2^2)*(x1^2) - (y1^2))*x1*y1/(2*(x2*y2*(x1^2) - (x2^2)*x1*y1)) - (((y2^2) - (y1^2)/(x1^2))*x1*y1/(x2*y2*(x1^2) - (x2^2)*x1*y1) - (y1^2)/(x1^2))*(x3^2);
	float numc = 1 - ((x1^2) - 1)*x3*y3/(2*(x2*y2*(x1^2) - (x2^2)*x1*y1)) - (1/(x1^2) - (1 - 1/(x1^2))*x1*y1/(x2*y2*(x1^2) - (x2^2)*x1*y1)) * (x3^2);
	float c = numc / divc;
	float b = (((x1^2) - 1)/(x2*y2*(x1^2) - (x2^2)*x1*y1) - c * ((y2^2) * (x1^2) - (y1^2))/(x2*y2*(x1^2) - (x2^2)*x1*y1))/2;
	float a = (1 - 2 * b * x1*y1 - c * (y1^2))/(x1^2);
	vector<float> eq = vector<float>();
	eq.push_back(a);
	eq.push_back(b);
	eq.push_back(c);
	return eq;
}

vector<float> checkEllipse(Mat img, vector<vector<Point> >* ret)
{
	namedWindow("Display1", WINDOW_AUTOSIZE);
	namedWindow("Display2", WINDOW_AUTOSIZE);
	namedWindow("DST", WINDOW_AUTOSIZE);
	vector<float> ratios = vector<float>();
	//vector<vector<Point> > ret;
	//findContours(img, ret, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point());
	for (uint i = 0; i < (*ret).size(); i++)
	{
		vector<Point> cnt = (*ret)[i];
		if (cnt.size() < 5)
			continue;
		RotatedRect rect = fitEllipse(cnt);
		Mat img1 = Mat::zeros(img.rows, img.cols, CV_8UC3);
		Mat img2 = Mat::zeros(img.rows, img.cols, CV_8UC3);
		Mat dst = Mat::zeros(img.rows, img.cols, CV_8UC3);
		drawContours(img1, *ret, i, Scalar(255,255,255), 1, 8, noArray(), INT_MAX, Point());
		ellipse(img2, rect, Scalar(255,255,255), 1, 8);
		absdiff(img1,img2, dst);
		imshow("Display1", img1);
		waitKey(0);
		imshow("Display2", img2);
		waitKey(0);
		imshow("DST", dst);
		waitKey(0);
		cvtColor(dst, dst, CV_RGB2GRAY);
		int n = countNonZero(dst);
		float ratio = (float)n / (float)(cnt.size() * 2);
		ratios.push_back(ratio);
	}
	return ratios;
}

vector<int> checkSquare(vector<vector<Point> >* ret)
{
	vector<int> ratios = vector<int>();
	for (uint i = 0; i < ret->size(); i++)
	{
		vector<Point> approx = vector<Point>();
		vector<Point> cnt = (*ret)[i];
		approxPolyDP(cnt, approx, arcLength(cnt, true) * 0.002, true);
		if (!isContourConvex(approx))
		{
			ratios.push_back(0);
			continue;
		}
		if (approx.size() == 4)
			ratios.push_back(1);
		else
			ratios.push_back(0);	
	}
	return ratios;
}

vector<int> checkTriangle(vector<vector<Point> >* ret)
{
	vector<int> triangles = vector<int>();
	for (uint i = 0; i < ret->size(); i++)
	{
		vector<Point> approx = vector<Point>(); 
		vector<Point> cnt = (*ret)[i];
		approxPolyDP(cnt, approx, arcLength(cnt, true) * 0.02, true);
		if (!isContourConvex(approx))
		{
			triangles.push_back(0);
			 continue;
		}
		if (approx.size() == 3)
			triangles.push_back(1);
		else
			triangles.push_back(0);			
	}
	return triangles;
}


//Tell if the form is a square(1), a triangle(2) or a circle(3), (0) is for nothing
int giveForm(int tri, float rcir, int rrec)
{
	if (tri)
		return 2;
	else
	{
		if (rcir < 0.30)
			return 3;
		else
		{
			if (rrec)
				return 1;
			else
				return 0;
		}
	}
}
