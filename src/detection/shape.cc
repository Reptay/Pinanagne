#include "shape.hh"

Circle* getCircle(Mat img){
  Circle *c = NULL;
  Mat img_gray;
  cvtColor(img, img_gray, CV_BGR2GRAY);

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur(img_gray, img_gray, Size(3, 3), 2, 2 );

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1,
	       img_gray.rows/8, 200, 100, 0, 0 );

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( img, center, radius, Scalar(0,0,255), 2, 8, 0 );
      c = new Circle(center, radius);
    }

  namedWindow("Display", WINDOW_AUTOSIZE);
  imshow("Display", img);
  return c;
}