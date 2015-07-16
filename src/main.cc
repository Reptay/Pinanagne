#include <string>
#include "filter/filters.hh"
#include "detection/shape.hh"

#include "ransac/ransac.hh"

void fluxWebcam(std::string path)
{
  CvCapture *capture;
  if (path.empty()) // pas testé pour la webcam
    capture = cvCreateCameraCapture( CV_CAP_ANY );
  else
    capture = cvCreateFileCapture(path.c_str());
  if (!capture) {
    printf("Ouverture du flux vidéo impossible !\n");
    exit(1);
  }

  char key;
  IplImage *image;
  // cvNamedWindow("Webcam", CV_WINDOW_AUTOSIZE);
  // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
  while(key != 'q' && key != 'Q') {
    // On récupère une image
    image = cvQueryFrame(capture);

    Mat img = Mat(image);
    getCircle(img);
    //cvShowImage( "Webcam", image);
    // On attend 10ms
    key = cvWaitKey(10);
 
  }
 
  cvReleaseCapture(&capture);
  //cvDestroyWindow("Webcam");

  exit(0);
}

void traitementImage(char* path)
{
  Mat img;
  img = imread(path, CV_LOAD_IMAGE_COLOR);
  if (img.data)
    {
      getCircle(img);
      waitKey(0);
      BlueRedFilter(img);
      getContour(img);
      vector<vector<Point> > ret;
      Point center = findcenter(img, &ret);

      namedWindow("Display", WINDOW_AUTOSIZE);
      imshow("Display", img);
      waitKey(0);
      cout << "x=" << center.x << "y=" << center.y << endl;
    }
  else
    exit(1);
}

int main(int argc, char* argv[])
{
  std::string video = "tests/video/nationale/1/panneau1.mp4";
  fluxWebcam(video);

 if (argc == 2)
 {
  Mat img;
  img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  if (img.data)
  {
   //getCircle(img);
   
   BlueRedFilter(img);
   getContour(img);
   //vector<vector<Point> > ret;
   //Point center = findcenter(img, &ret);
   /*Mat cpy(img);
   getContour(img);
   getOrientation(cpy);
   Mat end(cpy);*/
   //vector<float> eq = geteq(center, ret[0]);
   //cout << "A=" << eq[0] << "B=" << eq[1] << "C=" << eq[2] << endl;
   vector<float> ellipses = checkEllipse(img);
   namedWindow("Display", WINDOW_AUTOSIZE);
   imshow("Display", img);
   waitKey(0);
   //traitementImage(argv[1]);
   /*Mat cpy(img);
   getContour(img);
   getOrientation(cpy);
   Mat end(cpy);*/   

   /*imshow("Display", cpy);
   waitKey(0);
   findcenter(end,cpy);
   imshow("Display", end);
   waitKey(0);*/
   for (uint i = 0; i < ellipses.size(); i++)
	{
		cout << ellipses[i] << endl;
	}
 //  cout << "x=" << center.x << "y=" << center.y << endl;
   return 0;
 } else {
   return 1;
 }
 return 2;
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
