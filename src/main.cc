#include <string>
#include "filter/filters.hh"
#include "detection/shape.hh"
#include "detection/typePanneau.hh"
#include "ransac/ransac.hh"
#include "audio/audio.hh"
#include "surf/surf.hh"
#include <dirent.h>

void fluxWebcam(std::string path)
{
  //  path = "";
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
    Mat img = cvarrToMat(image);
    if (img.empty())
      break;
    
    double factor = 0.5;
    resize(img, img, Size(), factor, factor, cv::INTER_LANCZOS4);
    
    std::vector<Circle*> circles = getCircles(img);
    //    std::cerr << circles.size() << " ";

    std::vector<Mat> panneaux;
    for (std::vector<Circle*>::iterator it = circles.begin();
	 it != circles.end(); it++){
      Mat* m = isLimitation(img, *it);
      if (m != NULL){
	panneaux.push_back(*m);
	std::cerr <<std::endl<<
	  "----------------" <<"OK"<<"----------------" << std::endl;
      }
      (*it)->draw(img);
      /*
      namedWindow("Display2", WINDOW_AUTOSIZE);
      imshow("Display2", img);
      waitKey(0);
      */

    }
    for (std::vector<Mat>::iterator it = panneaux.begin();
	 it != panneaux.end(); it++){
      namedWindow("Display", WINDOW_AUTOSIZE);
      imshow("Display", *it);
      waitKey(0);
      // RANSAC ICI, normalement il y a un panneau au maximum dans le vector
    }

    IplImage image2=img;
    cvShowImage( "Webcam", &image2);
    // On attend 10ms
    key = cvWaitKey(1);
 
  }
 
  cvReleaseCapture(&capture);
  //cvDestroyWindow("Webcam");

  exit(0);
}

void ReadWebcam(char* path)
{
	std::string s(path);
	VideoCapture v(s);
	std::cout << s << std::endl;
	if (!v.isOpened())
		return;
	for(;;)
	{
		Mat frame;
		v >> frame;
		imshow("frame", frame);
		waitKey();
	}
}

void ReadWebcam(int device)
{
	VideoCapture v = VideoCapture(device);
	for(;;)
	{
		Mat frame;
		v >> frame;
		imshow("frame", frame);
		waitKey();
	}
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
	/*DIR* d;
	  d = opendir("./modeles");
		if (!d)
			return;
		struct dirent* f;
		while ((f = readdir(d)))
		{
			string st = "./modeles";
			st += f->d_name;
			Mat mod = imread(st);
			if (img.data)
				{
					Mat outImg = *it;
					if (outImg.data)
{
					findObject(*it, mod, 2500, Scalar(255,0,0), outImg);
					imshow("Test", outImg);
waitKey(0);
}
				}
		}*/
		Mat dst = *it;
		pyrUp(*it, dst, Size(dst.cols * 2, dst.rows * 2));
		Mat dest = dst;
		pyrUp(dst, dest, Size(dst.cols * 2, dst.rows * 2));
		//dst = dest;
		//pyrUp(dest, dst, Size(dest.cols*2, dest.rows*2));
		Mat mod = imread("./modeles/110km.jpg");
		Mat outImg = dest;
		findObject(dest, mod, 2500, Scalar(255,0,0), outImg);
		imshow("Test", outImg);
waitKey(0);
	/*namedWindow("Display", WINDOW_AUTOSIZE);
	imshow("Display", *it);
	waitKey(0);*/
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
    {
      std::cerr << "Ouverture de l'image impossible" << std::endl;
      exit(1);
    }
}

int main(int argc, char* argv[])
{ 
  //playSound("audio/50.wav");
  if (argc == 1)
    fluxWebcam(""); //webcam
  else if (argc == 2)
    ReadWebcam(argv[1]);
  else if (argc == 3 && strcmp(argv[1], "-i")==0)
    {
      Mat img;
      img = imread(argv[2], CV_LOAD_IMAGE_COLOR);
      if (img.data)
	{
	  traitementImage(argv[2]);
	//Comparaison avec les modeles via un surf
	  	  //findObject(argv[2], 
	  return 0;
	} else {
	return 1;
      }
      return 2;
    }
  else
    std::cerr << "Invalide argument" << std::endl;
}
