#include "filters.hh"

void RedFilter(Mat img)
{
	for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		{
		  Point3_<uchar>* p = img.ptr<Point3_<uchar> >(j,i);

			int r = p->z;
			int g = p->y;
			int b = p->x;
			if (r > 2*(g+b) && r > 30)
			{
				p->z = 0;
				p->x = 0;
				p->y = 0;
			}
			else
			{
				p->z = 255;
				p->y = 255;
				p->x = 255;
			}
		}
}

void BlueFilter(Mat img)
{
	for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		{
			Point3_<uchar>* p = img.ptr<Point3_<uchar> >(i,j);
			if (p->x > 200 && p->z < p->x && p->y < p->x)
			{
				p->z = 0;
				p->x = 0;
				p->y = 0;
			}
			else
			{
				p->z = 255;
				p->y = 255;
				p->x = 255;
			}
		}
}

void To_Lab(Mat img)
{
	//Mat imgLab;
	cvtColor(img, img, CV_BGR2Lab);
	//return imgLab;
}

void BlueRedFilter(Mat img)
{
	for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		{
			Point3_<uchar>* p = img.ptr<Point3_<uchar> >(j,i);
			double b = p->x;
			double g = p->y;
			double r = p->z;
			if (r / b > 1 && r/g > 1)
			{
				p->z = 0;
				p->x = 0;
				p->y = 0;
			}
			else if (b / r > 1 && b / g > 1)
			{
				p->z = 0;
				p->y = 0;
				p->x = 0;
			}
			else 
			{
				p->x = 255;
				p->y = 255;
				p->z = 255;
			}
		}

}

void To_TSV(Mat img)
{
	cvtColor(img, img, CV_BGR2HSV, 0);
}

void getContour(Mat& img)
{
  cvtColor(img, img, CV_BGR2GRAY);
  Canny(img,img,50,1500,3);
}

void getOrientation(Mat& img)
{
	Mat Sx, Sy, mag;
    Sobel(img, Sx, CV_32F, 1, 0, 3);
	Sobel(img, Sy, CV_32F, 0, 1, 3);
	magnitude(Sx, Sy, mag);
	phase(Sx, Sy, img, true);
    
}




Point findcenter(Mat img, vector<vector<Point> >* ret)
{
	findContours(img, *ret, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point());
	vector<Point> cnt = (*ret)[0];
	Moments m = moments(cnt, true);
	int cx = m.m10/m.m00;
	int cy = m.m01/m.m00;
	return Point(cx, cy);
}
