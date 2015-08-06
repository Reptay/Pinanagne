#include <string>
#include "filter/filters.hh"
#include "detection/shape.hh"
#include "detection/typePanneau.hh"
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
    //    std::vector<Circle*> circles = getCircle(img);
    /* Si c != NULL alors RANSAC */

    //cvShowImage( "Webcam", image);
    // On attend 10ms
    key = cvWaitKey(1);
 
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
      /******* Detection d'un panneau de limitation *******/
      std::vector<Circle*> circles = getCircles(img);
      std::vector<Mat> panneaux;
      for (std::vector<Circle*>::iterator it = circles.begin();
	   it != circles.end(); it++){
	Mat* m = isLimitation(img, *it);
	if (m != NULL)
	  panneaux.push_back(*m);
      }
      for (std::vector<Mat>::iterator it = panneaux.begin();
           it != panneaux.end(); it++){
	namedWindow("Display", WINDOW_AUTOSIZE);
	imshow("Display", *it);
	waitKey(0);
      }
      exit(0);
      //c->draw(img);
      /*BlueRedFilter(img);
      getContour(img);
      blur(img, img, Size(20,20));
      namedWindow("Display", WINDOW_AUTOSIZE);
      imshow("Display", img);
      waitKey(0);*/
      /***************************************************/
      /******* Lecture du panneau *******/
      /*
      vector<vector<Point> > ret;
	  cvtColor(img, img, CV_BGR2GRAY);
      Point center = findcenter(img, &ret);
	  vector<float> ellipses = checkEllipse(img, &ret);
	  vector<int> squares = checkSquare(&ret);
	  vector<int> triangles = checkTriangle(&ret);

	for (uint i = 0; i < ellipses.size(); i++)
	{
		int val = giveForm(triangles[i], ellipses[i], squares[i]);
		cout << val << endl;
	}
      cout << "x=" << center.x << "y=" << center.y << endl;
      */
      /***************************************************/
    }
  else
    exit(1);
}

int main(int argc, char* argv[])
{/*
  std::string video = "tests/video/nationale/1/panneau1.mp4";
  fluxWebcam(video);
 */
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
