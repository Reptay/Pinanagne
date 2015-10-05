#include "shape.hh"

using namespace cv;
using namespace std;


std::vector<Circle*> getCircles(Mat img){
  std::vector<Circle*> vCircles;
  double factorSize = 1;
  Mat img_gray;  
  //cvtColor(img, img_gray, CV_BGR2GRAY);
  /*
  if(img.empty())
    return std::vector<Circle*>();
  else if(img.channels()>1)
    cvtColor(img, img_gray, CV_BGR2GRAY);
  else img_gray = img;
  */
  
  img_gray = RedFilterSouple(img);
  while (nbWhitePix(img_gray) > 30000){
    factorSize /= 2;
    resize(img_gray, img_gray, Size(), factorSize, factorSize, cv::INTER_LANCZOS4);
  }
  
  /// Reduce the noise so we avoid false circle detection
  GaussianBlur(img_gray, img_gray, Size(9,9), 2, 2);

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1.2,
  	       img_gray.rows/8,180, 100, 0, 0 );
  //                           ^^^ parametre important

  /// Draw the circles detected

  for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      //    circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );

      // circle outline
      //  circle( img, center, radius, Scalar(0,0,255), 2, 8, 0 );
      if (factorSize == 1)
	vCircles.push_back(new Circle(center, radius));
      else
	vCircles.push_back(new Circle(center, radius, factorSize));
    }
  /*
  namedWindow("Display", WINDOW_AUTOSIZE);
  imshow("Display", img);
  */
  return vCircles;
}

std::vector<Circle*> getCirclesByEllipses(Mat src)
{
  int thresh = 100;
  RNG rng(12345);
  std::vector<Rectangle> rects;

  Mat src_gray;
  src_gray = RedFilterSouple(src);
  blur( src_gray, src_gray, Size(3,3) );
	       
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect( contours.size() );
  vector<RotatedRect> minEllipse( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
    { minRect[i] = minAreaRect( Mat(contours[i]) );
      if( contours[i].size() > 5 )
	{ minEllipse[i] = fitEllipse( Mat(contours[i]) ); }
    }

  /// Draw contours + rotated rects + ellipses
  Mat drawing;// = Mat::zeros( threshold_output.size(), CV_8UC3 );
  drawing = src.clone();
  for( int i = 0; i< contours.size(); i++ )
    {
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      // contour
      //      drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // ellipse
      //ellipse( drawing, minEllipse[i], color, 2, 8 );
      // rotated rectangle
      Point2f rect_points[4]; minRect[i].points( rect_points );
      Rectangle r(rect_points);
      rects.push_back(r);
    }

  /// Show in a window
  /*  
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
  waitKey(0);
  */
  std::vector<Circle*> circles;
  for (int i = 0; i < rects.size(); i++)
    {
      if (rects[i].isCircleProportion())
	circles.push_back(rects[i].getCircle());
    }
  
  // Supprime les cercles dans d'autres cercles
  for (std::vector<Circle*>::iterator it1 = circles.begin();
       it1 != circles.end();){
    bool brk = false;
    for(std::vector<Circle*>::iterator it2 = circles.begin();
	it2 != circles.end(); it2++){
      if (it1 == it2 && it1 != circles.end())
	continue;
      if ((*it1)->isInside(*it2)){
	it1 = circles.erase(it1);
	brk = true;
	break;
      }
    }
    if (!brk)
      it1++;
  }
  
  return circles;
}


