#include "detectRect.hh"

static double angle(Point pt1, Point pt2, Point pt0)
{
double dx1 = pt1.x -pt0.x;
double dx2 = pt2.x - pt0.x;
double dy1 = pt1.y - pt0.y;
double dy2 = pt2.y - pt0.y;
return (dx1 * dx2 + dy1 * dy2)/sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

vector<Rect> detectRect(Mat img)
{
//utilisation du redfilter doit etre utilisée avant l'appel a dRect pour pouvoir autoroute et ville sur la meme fonction
vector<Rect> rects = vector<Rect>();
vector<vector<Point> > contours;
findContours(img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
for (int i = 0; i < contours.size(); i++)
{
vector<Point> approx;
approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
{
double maxcos = 0;
for (int j = 2; j < 5; j++)
{
double cosin = fabs(angle(approx[j%4], approx[j - 2], approx[j- 1]));
maxcos = MAX(maxcos, cosin);
}
if (maxcos < 0.3)
rects.push_back(Rect(approx[0], approx[2]));
//rectangle(img, approx[0], approx[2], Scalar(0,0,0), 1, 8, 0); 
}

}
return rects;
}

bool isCity(Mat img)
{
Mat red = RedFilter(img);
return detectRect(red).size() > 0;
}

bool isHighWay(Mat img)
{
Mat blue = BlueFilter(img);
//imshow("blue", blue);
//waitKey();
vector<Rect> rects = detectRect(blue);
//cout << "size: " << rects.size() << endl;
for (int i = 0; i < rects.size(); i++)
{
Mat interimg = Mat (img, rects[i]);
Mat label = imread("./modeles/autoroutes/autoroute.jpg");
Mat outimg = img;
int n = findObject(interimg, label, 500, Scalar(255,0,0), outimg);
//waitKey();
if (n > 0)
return true;
}
return false;
}

bool endCity(Mat img)
{
Mat black = BlackFilter(img);
vector<Rect> rects = detectRect(black);
for (int i = 0; i < rects.size(); i++)
{
Mat interimg = Mat(img, rects[i]);
Mat red = RedFilter(interimg);
if (nbWhitePix(red) > 0)
return true;
}
return false;
}

bool endHighWay(Mat img)
{
Mat label = imread("./modeles/autoroutes/fin.png");
Mat outimg = img;
int n = findObject(img, label, 500, Scalar(255,0,0), outimg);
return n > 0;
}

vector<RotatedRect> LinestoRect(Mat img)
{
	vector<RotatedRect> rects = vector<RotatedRect>();
	vector<vector<Point> > contours;
	findContours(img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
{

	RotatedRect r = minAreaRect(contours[i]);
	Point2f rect_points[4];
	r.points(rect_points);
	int bl = 0;
	for (int j = 0; j < 4; j ++)
	{
		double x = rect_points[j].x;
		double y = rect_points[j].y;
		if (y > 0 && x - 20 > 0 && img.at<Vec3b>(y, x -20).val[0] == 0)
			bl++;
	}
	double a = r.angle;
	double h = r.size.height;
	double w = r.size.width;
if (h * w > 2 && h * w < 500 && bl >= 2 && 
(( w > h && abs(a) >= 20 && w/h > 1 && w/h < 25) || 
(h > w && abs(a) <= 70 &&  h/w > 1 && h/w < 25)))
	rects.push_back(r);

}
return rects;
}

void printlines(Mat img, int fi)
{
Mat dst;
cvtColor(img, dst, CV_GRAY2BGR);
if (fi == 0)
{
//Hough Transform
	vector<Vec2f> lines;
	HoughLines(img, lines, 1, CV_PI/180, 100, 0, 0);

for (int j = 0; j < lines.size(); j++)
{
	float rho = lines[j][0], theta = lines[j][1];
	double a = cos(theta), b = sin(theta);
	double x0 = a * rho, y0 = b * rho;
	Point p1, p2;
	p1.x = cvRound(x0 - b * 1000);
	p1.y = cvRound(y0 + 1000 * a);
	p2.x = cvRound(x0 + b * 1000);
	p2.y = cvRound(y0 - a * 1000);
	line(dst, p1, p2, Scalar(255,0,0), 1, 8);
}
}
else
{
	if (fi == 1)
	{
		vector<Vec4i> lines;
		HoughLinesP(img, lines, 1, CV_PI/180, 50, 50, 10);
		for (int j = 0; j < lines.size(); j++)
		{
			Vec4i l = lines[j];
			Point p1 = Point(l[0], l[1]);
			Point p2 = Point(l[2], l[3]);
			line(dst, p1, p2, Scalar(255, 0, 0), 1, 8);
		}
	}
	else
		return;
}
	imshow("test", dst);
	waitKey(); 
}

int getSeuil(Mat img)
{
	int col = 0;
	if (img.data)
	{
		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
			{
				col += img.data[img.step[0] * i + img.step[1] * j];
			}
	}
	col = col / (img.rows * img.cols);
	/*if (col > 100)
	{
	float z = col * 1.33;
	col = (int) z;
	}
	else
	{
		col = 100;
	}*/
	col += 50;
	return col;
}

vector<RotatedRect> getLines(Mat img)
{
//copie necessaire?
	//Decoupe image
	Mat cpy = Mat(img, Rect(0, img.rows / 2, img.cols, img.rows / 2));
	/*Mat test = WhiteFilter(cpy);
	imshow("youpla", test);
	waitKey();*/
	//cvtColor(cpy, cpy, CV_HSV2BGR);
	cvtColor(cpy, cpy, CV_BGR2GRAY);
	int s = getSeuil(cpy);
	//std::cout << s << endl;
	threshold(cpy, cpy, s, 255, THRESH_BINARY);
	
//Fonction de test pour juste les lignes(0 hough, 1 hough proba, autre entier rien)
	printlines(cpy, 2);

	//img = WhiteFilter(img);
	/*imshow("test", img);
	waitKey();*/
	//Mat copy = Mat(img, Rect(0,0,img.rows, img.cols)); 
	return LinestoRect(cpy);
}
