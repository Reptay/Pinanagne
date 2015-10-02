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
vector<Rect> rects = detectRect(blue);
for (int i = 0; i < rects.size(); i++)
{
Mat interimg = Mat (img, rects[i]);
Mat label = imread("./modeles/autoroute.jpg");
Mat outimg = img;
int n = findObject(interimg, label, 500, Scalar(255,0,0), outimg);
waitKey();
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
