#include "filters.hh"


int main(int argc, char* argv[])
{
 if (argc == 2)
 {
  Mat img;
  img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  if (img.data)
  {
   BlueRedFilter(img);
   getContour(img);
   vector<vector<Point> > ret;
   Point center = findcenter(img, &ret);
   /*Mat cpy(img);
   getContour(img);
   getOrientation(cpy);
   Mat end(cpy);*/
   
   namedWindow("Display", WINDOW_AUTOSIZE);
   imshow("Display", img);
   waitKey(0);
   /*imshow("Display", cpy);
   waitKey(0);
   findcenter(end,cpy);
   imshow("Display", end);
   waitKey(0);*/
   cout << "x=" << center.x << "y=" << center.y << endl;
   return 0;
  }
 return 1;
 }
 /* Mat test = Mat::zeros(Size(320,420), CV_8UC1);
  circle(test, Point(160,120), 80, Scalar(255,255,255), -1, CV_AA);
  getOrientation(test);
  findcenter(test,test);
  vector<vector<Point> > ret;
  findContours(test, ret, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point());
  OutputArray cnt = ret[0];
  Moments m = moments(cnt, true);
  int cx = m.m10/m.m00;
  int cy = m.m01/m.m00;
  cout << "cx=" << cx << "cy=" << cy << endl;
  namedWindow("Test", WINDOW_AUTOSIZE);
  imshow("Test", test);
  waitKey(0);*/
 return 2;
}
