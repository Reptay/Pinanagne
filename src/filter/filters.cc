#include "filters.hh"

void RedFilter(Mat img)
{
  /*
  Mat hsv = img.clone();
  cvtColor(img, hsv, CV_BGR2HSV);

  Mat mask1(img.size(), CV_8UC1);
  Mat mask2(img.size(), CV_8UC1);

  


  int sMin = 150;
  int sMax = 255;
  int vMin = 90;
  int vMax = 255;
  int hTol=10;

  inRange(hsv, Scalar(0, sMin, vMin), Scalar(hTol, sMax, vMax), mask1);
  inRange(hsv, Scalar(180-hTol, sMin, vMin), Scalar(180, sMax, vMax), mask2);

  Mat mask = mask1+mask2;

  namedWindow("DisplayRed", WINDOW_AUTOSIZE);
  imshow("DisplayRed", mask);
  waitKey(0);

  return;
*/
	for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		{
		  Point3_<uchar>* p = img.ptr<Point3_<uchar> >(j,i);

			int r = p->z;
			int g = p->y;
			int b = p->x;
			double diff;
			if  (b==0||g==0)
			  diff = 2;
			else if (b>g)
			  diff = b/g;
			else
			  diff = g/b;
			if ((r > 2*(g+b) && r > 30) ||
			    (r>80 && r>g+b) ||
			    (r>120 && r>(g+b)*0.8)||
			    (r>130 && r>g*0.9 &&r>b*0.9&&g < 150 &&
			     b < 150 && diff < 1.3))
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

void BlackFilter(Mat img)
{
  for (int i = 0; i < img.cols; i++)
    for (int j = 0; j < img.rows; j++)
      {
	Point3_<uchar>* p = img.ptr<Point3_<uchar> >(j,i);

	int r = p->z;
	int g = p->y;
	int b = p->x;
	double diffrg = (r>g) ? r/g : g/r;
	double diffrb = (r>b) ? r/b : b/r;
	double diffgb = (g>b) ? g/b : b/g;

	if ((r+g+b < 50 && r+g>b && r+b>g && g+b>r) ||
	    (r+g+b<30) ||
	    (r < 50 && b < 50 && g < 50) ||
	    (r<100 && b < 100 && g < 100 && r+g>b && r+b>g && g+b>r)||
	    (diffrg < 1.3 && diffrb < 1.3 && diffgb < 1.3 && r+g+b<350))
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
