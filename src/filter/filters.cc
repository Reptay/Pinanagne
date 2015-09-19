#include "filters.hh"

void dp(Mat img){
  namedWindow("DisplayDp", WINDOW_AUTOSIZE);
  imshow("DisplayDp", img);
  waitKey(0);
}
void dp(Mat img, std::string str){
  namedWindow("Display " + str, WINDOW_AUTOSIZE);
  imshow("Display " + str, img);
  waitKey(0);
}

Mat RedFilter(Mat img)
{
  /*
  Mat hsv = img.clone();
  cvtColor(img, hsv, CV_BGR2HSV);

  Mat mask1(img.size(), CV_8UC1);
  Mat mask2(img.size(), CV_8UC1);

  int sMin = 100;
  int sMax = 255;
  int vMin = 30;
  int vMax = 255;
  int hTol=15;

  inRange(hsv, Scalar(0, sMin, vMin), Scalar(hTol, sMax, vMax), mask1);
  inRange(hsv, Scalar(180-hTol, sMin, vMin), Scalar(180, sMax, vMax), mask2);

  Mat mask = mask1+mask2;
  //dp(mask);
  return mask;
*/
  Mat hsv = img.clone();
  cvtColor(img, hsv, CV_BGR2HSV);

  Mat mask1(img.size(), CV_8UC1);
  Mat mask2(img.size(), CV_8UC1);
  Mat mask3(img.size(), CV_8UC1);
  Mat mask4(img.size(), CV_8UC1);
  Mat mask5(img.size(), CV_8UC1);
  Mat mask6(img.size(), CV_8UC1);

  int sMin = 120;
  int sMax = 255;
  int vMin = 50;
  int vMax = 255;
  int hTol=15;

  inRange(hsv, Scalar(0, sMin, vMin), Scalar(hTol, sMax, vMax), mask1);
  inRange(hsv, Scalar(180-hTol, sMin, vMin), Scalar(180, sMax, vMax), mask2);

  sMin = 50;
  vMin = 30;
  hTol=10;
  inRange(hsv, Scalar(0, sMin, vMin), Scalar(hTol, sMax, vMax), mask3);
  inRange(hsv, Scalar(180-hTol, sMin, vMin), Scalar(180, sMax, vMax), mask4);

  sMin = 20;
  vMin = 15;
  hTol=5;
  inRange(hsv, Scalar(0, sMin, vMin), Scalar(hTol, sMax, vMax), mask5);
  inRange(hsv, Scalar(180-hTol, sMin, vMin), Scalar(180, sMax, vMax), mask6);

  Mat mask = mask1+mask2+mask3+mask4+mask5+mask6;
  //  dp(mask);
  return mask;
}

Mat RedFilterSouple(Mat img)
{
  return RedFilter(img);
  /*  Mat hsv = img.clone();
  cvtColor(img, hsv, CV_BGR2HSV);

  Mat mask1(img.size(), CV_8UC1);
  Mat mask2(img.size(), CV_8UC1);

  int sMin = 60;
  int sMax = 255;
  int vMin = 20;
  int vMax = 255;
  int hTol=20;

  inRange(hsv, Scalar(0, sMin, vMin), Scalar(hTol, sMax, vMax), mask1);
  inRange(hsv, Scalar(180-hTol, sMin, vMin), Scalar(180, sMax, vMax), mask2);

  Mat mask = mask1+mask2;
  //dp(mask);
  return mask;*/
}

int moyenneValue(Mat img)
{
  long long int totalValue = 0;
  long long int nbPoint = img.cols*img.rows;
  
  for (int i = 0; i < img.cols; i++)
    for (int j = 0; j < img.rows; j++)
      {
	Vec3b hsv=img.at<Vec3b>(j,i);
	int H=hsv.val[0]; //hue
	int S=hsv.val[1]; //saturation
	int V=hsv.val[2]; //value
	if (V < 250) // pas totalement blanc
	  totalValue += V;
	else
	  nbPoint--;
      }
  return (int) totalValue / nbPoint;
}


Mat BlackFilter(Mat img)
{
  Mat hsv = img.clone();
  cvtColor(img, hsv, CV_BGR2HSV);

  int moyValue = moyenneValue(hsv);
  //  std::cout << "-----> " << moyValue << std::endl;

  Mat mask1(img.size(), CV_8UC1);
  int sMin = 0;
  int sMax = 255;
  int vMin = 0;
  int vMax = moyValue/2; //70

  inRange(hsv, Scalar(0, sMin, vMin), Scalar(180, sMax, vMax), mask1);
  //  dp(mask1);
  return mask1;

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
/*
Mat BlackFilter(Mat img)
{
 
  for (int i = 0; i < img.cols; i++)
    for (int j = 0; j < img.rows; j++)
      {
	Point3_<uchar>* p = img.ptr<Point3_<uchar> >(j,i);

	int r = p->z;
	int g = p->y;
	int b = p->x;
	/ *
	double diffrg = 1;
	if (r != 0 && g != 0)
	  diffrg = (r>g) ? r/g : g/r;
	double diffrb = 1;
	if (r != 0 && b != 0)
	  diffrb=(r>b) ? r/b : b/r;
	double diffgb = 1;
	if (g != 0 && b != 0)
	  diffgb = (g>b) ? g/b : b/g;
	* /
	if ((r+g+b < 50 && r+g>b && r+b>g && g+b>r) ||
	    (r+g+b<30)) / *||
	    (r < 50 && b < 50 && g < 50) ||
	    (r<100 && b < 100 && g < 100 && r+g>b && r+b>g && g+b>r)
	    ||(diffrg < 1.3 && diffrb < 1.3 && diffgb < 1.3 && r+g+b<350))* /
	  {
	    p->z = 255;
	    p->x = 255;
	    p->y = 255;
	  }
	else
	  {
	    p->z = 0;
	    p->y = 0;
	    p->x = 0;
	  }

      }
  
      }*/

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


int nbWhitePix(Mat img){
  int nb = 0;
  for (int i = 0; i < img.cols; i++)
    for (int j = 0; j < img.rows; j++)
      {
        int pImg = (int)img.at<uchar>(j,i);
	if (pImg == 255)
	  nb++;
      }
  return nb;
}
