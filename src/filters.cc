#include "filters.hh"

void RedFilter(Mat img)
{
	for (int i = 0; i < img.cols; i++)
		for (int j = 0; j < img.rows; j++)
		{
			Point3_<uchar>* p = img.ptr<Point3_<uchar> >(i,j);
			if (p->z > 200 && p->x < p->z && p->y < p->z)
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
			Point3_<uchar>* p = img.ptr<Point3_<uchar> >(i,j);
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
	Canny(img,img,10,1000,3);
}

void getOrientation(Mat& img)
{
	Mat Sx, Sy, mag, ori;
    Sobel(img, Sx, CV_32F, 1, 0, 3);
	Sobel(img, Sy, CV_32F, 0, 1, 3);
	magnitude(Sx, Sy, mag);
	phase(Sx, Sy, img, true);
    
}

void Drawligne(Mat img, int x, int y, int a, Scalar col)
{
 int length = 1;
 while (length * cos(a * 3.14/180) + x < img.cols && length * cos(a * 3.14/180) + x > 0)
	length ++;
 length --;
 int x2 = length * cos(a * 3.14/180) + x;
 while (length * sin(a * 3.14/180) + y < img.rows && length * sin(a * 3.14/180) + y > 0)
	length++;
 length --;
 int y2 = length * sin(a * 3.14/180) + y;
 Point p1 = Point(x,y);
 Point p2 = Point(x2, y2);
 line(img, p1, p2, col, 2, 8);
}


void findcenter(Mat img, Mat ori)
{
 int x;
 int y;
 Point3_<uchar>* p1,* p2,* p3;
 float* p4,* p5,* p6;
 while (!p3)
 {
	x = rand() % img.cols;
	y = rand() % img.rows;
	if (!p1)
	{
	 p1 = img.ptr<Point3_<uchar> >(x, y);
	 if (p1->x != 255)
		p1 = NULL;
	 else
	   {
		p4 = (float*) ori.data + (img.cols*y + x) * sizeof(float);
		Drawligne(img, x, y, (int) *p4, Scalar(255,0,0));
	   }
	}
    else if (!p2)
	{
	 p2 = img.ptr<Point3_<uchar> >(x, y);
	 if (p2->x != 255)
		p2 = NULL;
	 else
	   {
		p5 = (float*) ori.data + (img.cols*y + x) * sizeof(float);
		Drawligne(img, x, y, (int) *p5, Scalar(0,255,0));
	   }
	}
    else
	{
	 p3 = img.ptr<Point3_<uchar> >(x, y);
	 if (p3->x != 255)
		p3 = NULL;
	 else
	   {
		p6 = (float*) ori.data + (img.cols*y + x) * sizeof(float);
		Drawligne(img, x, y, (int) *p6, Scalar(0,0,255));
	   }
	}
 }
 Point3_<uchar>* p7, p8, p9;
 int length = 1;  
}
