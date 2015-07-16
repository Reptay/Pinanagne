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
	  vector<float> ellipses = checkEllipse(img, &ret);
      namedWindow("Display", WINDOW_AUTOSIZE);
      imshow("Display", img);
      waitKey(0);
	for (uint i = 0; i < ellipses.size(); i++)
	{
		cout << ellipses[i] << endl;
	}

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
	traitementImage(argv[1]);
   return 0;
 } else {
   return 1;
 }
 return 2;
}
}


 
