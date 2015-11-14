#include <cmath>
#include <string>
#include <vector>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include "filter/filters.hh"
#include "detection/shape.hh"
#include "detection/typePanneau.hh"
#include "ransac/ransac.hh"
#include "audio/audio.hh"
#include "surf/surf.hh"
#include "detection/detectRect.hh"
#include <dirent.h>

std::mutex mtx;
Mat*  paralIsLimitation(Mat img, Circle* circle, std::vector<Mat>* panneaux)
{
  Mat* m = isLimitation(img, circle);
  if (m != NULL)
    {
      mtx.lock();
      panneaux->push_back(*m);
      circle->draw(img,0,255,0);
      mtx.unlock();
    }
  else
    {
      mtx.lock();
      circle->draw(img);
      mtx.unlock();
    }
}

bool fluxWebcam(std::string path)
{
  bool panneauDetecte = false;
  //  path = "";
  CvCapture *capture;
  if (path.empty()) // pas testé pour la webcam
    capture = cvCreateCameraCapture( CV_CAP_ANY );
  else
    capture = cvCreateFileCapture(path.c_str());
  if (!capture) {
    printf("Ouverture du flux vidéo impossible !\n");
    std::cout << path << std::endl;
    exit(1);
  }
  
  char key;
  IplImage *image;
  // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
  while(key != 'q' && key != 'Q') {
    // On récupère une image
    image = cvQueryFrame(capture);
    Mat img = cvarrToMat(image);
    if (img.empty())
      break;
    
    // std::vector<Circle*> circles = getCircles(img);
    std::vector<Circle*> circles=getCirclesByEllipses(img.clone());
    std::vector<Mat> panneaux;


    if (circles.size() > 1){ // PARALLELE
      std::thread threads[circles.size()];
      int iter = 0;    
      for (std::vector<Circle*>::iterator it = circles.begin();
	   it != circles.end(); it++){
	threads[iter] = std::thread(paralIsLimitation, img, *it,&panneaux);
	iter++;
      }
      for (auto& th : threads) th.join();
    }
    else{ // SEQUENTIEL
      for (std::vector<Circle*>::iterator it = circles.begin();
	   it != circles.end(); it++){
	Mat* m = isLimitation(img, *it);
	if (m != NULL){
	  panneaux.push_back(*m); // panneau detecte
	  (*it)->draw(img,0,255,0);
	}
	else
	  (*it)->draw(img);      
      }
    }
    if (panneaux.size() > 0)
      panneauDetecte=true;

    for (std::vector<Mat>::iterator it = panneaux.begin();
	 it != panneaux.end(); it++){
      namedWindow("Display", WINDOW_AUTOSIZE);
      imshow("Display", *it);
      //waitKey(0);
      // RANSAC ICI, normalement il y a un panneau au maximum dans le vector
    }
    
    IplImage image2=img;
    cvShowImage( "Webcam", &image2);
    //waitKey(0); 
    // On attend 10ms
    key = cvWaitKey(1);
  }
  
  cvReleaseCapture(&capture);
  //cvDestroyWindow("Webcam");
  //  exit(0);
  return panneauDetecte;
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

//Return vitzone * 1000 + maxvit
//vitzone => vitesse dans la zone
//maxvit => vitesse maximale autorisée

int traitementImage(char* path)
{
	int vitzone = 90;
	Mat img;
	img = imread(path, CV_LOAD_IMAGE_COLOR);

	if (img.data)
	{
		if (vitzone == 90 && isCity(img))
			vitzone = 50;
		else if (vitzone == 90 && isHighWay(img))
			vitzone = 130;
		else if (vitzone == 50 && endCity(img))
			vitzone = 90;
cout << "Vitesse maximale dans cette zone: " << vitzone << endl;
		/******* Detection d'un panneau de limitation *******/
		std::vector<Circle*> circles = getCirclesByEllipses(img);
		std::vector<Mat> panneaux;
		for (std::vector<Circle*>::iterator it = circles.begin();
				it != circles.end(); it++){
			Mat* m = isLimitation(img, *it);
			if (m != NULL)
				panneaux.push_back(*m);
		}
		std::vector<int> matches = std::vector<int>();
		for (std::vector<Mat>::iterator it = panneaux.begin();
				it != panneaux.end(); it++){
			DIR* d;
							cvtColor(*it, *it, CV_BGR2GRAY);
			d = opendir("./modeles/vitesse");
			if (!d)
				return;
			struct dirent* f;
			while ((f = readdir(d)))
			{
				string st = "./modeles/vitesse/";
				if (f->d_name[0] != '.')
				{
					st += f->d_name;
					cout << st << endl;
					Mat mod = imread(st);
					if (img.data)
					{
						Mat outImg = *it;
						if (outImg.data)
						{
							cvtColor(mod, mod, CV_BGR2GRAY);
							int n = findObject(*it, mod, 500, Scalar(255,0,0), outImg);
							matches.push_back(n);
							cout <<"nombre match: "<< n << endl;
	//						imshow("Test", outImg);
	//						waitKey(0);
						}
					}
				}
			}
			int minpos = 0;
			for (int i = 1; i < matches.size(); i++)
			{
				if (matches[i] > matches[minpos])
					minpos = i;
}
cout << "pos min "<< minpos << endl;
int maxvit;
switch(minpos)
{
	case 0: maxvit = 130; break;
	case 1: maxvit = 90; break;
	case 2: maxvit = 30; break;
	case 3: maxvit = 70; break;
	case 4: maxvit = 110; break;
	default: maxvit = 50;
}
cout << "Vitesse maximale autorisée: " << maxvit << endl;
return vitzone*1000 + maxvit;
			//Mat mod = imread("./modeles/90km6.png");
			//Mat dst = *it;
			//Mat tmp = dst;
			/*		while (dst.cols < mod.cols && dst.rows < mod.rows)
					{
					pyrUp(tmp, dst, Size(dst.cols * 2, dst.rows * 2));
					tmp = dst;
					}*/
			//Mat dest = dst;
			//pyrUp(dst, dest, Size(dst.cols * 2, dst.rows * 2));
			//dst = dest;
			//pyrUp(dest, dst, Size(dest.cols*2, dest.rows*2));
			/*		Mat outImg = dst;
					int n = findObject(dst, mod, 2500, Scalar(255,0,0), outImg);
					matches.push_back(n);		
					imshow("Test", outImg);
					waitKey(0);*/
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
  SDL_Init(SDL_INIT_AUDIO);
  //playLimitation("50");

  if (argc == 1)
    fluxWebcam(""); //webcam
  else if (argc == 2)
    fluxWebcam(argv[1]);
  else if (argc == 3 && strcmp(argv[1], "-f")==0) // dossier de vidéos en parametre
    {
      std::vector<std::string> filenameNonDetecte;
      int nbVideos = 0;
      int nbDetecte = 0;
      DIR *dir;
      struct dirent *ent;
      if ((dir = opendir (argv[2])) != NULL) {
	std::string directory(argv[2]);
	if (directory.back() != '/')
	  directory += "/";
	while ((ent = readdir (dir)) != NULL) {
	  if (ent->d_type == DT_REG){
	    nbVideos++;
	    printf ("%s\n", ent->d_name);
	    std::string filename = directory+ent->d_name;
	    std::cout << filename << std::endl;
	    bool detecte = fluxWebcam(filename);
	    if (detecte)
	      nbDetecte ++;
	    else
	      filenameNonDetecte.push_back(filename);
	  }
	}
	closedir (dir);
      } else {
	/* could not open directory */
	perror ("");
	return EXIT_FAILURE;
      }
      std::cout << "Vidéos où panneaux non detectées : " << std::endl;
      for (std::vector<std::string>::iterator it = filenameNonDetecte.begin() ;
	   it != filenameNonDetecte.end(); ++it)
	std::cout << "- " << *it << std::endl;
      std::cout << nbVideos << " vidéos et " << nbDetecte << " panneaux detectés"
		<< std::endl;
    }
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

  Mix_CloseAudio();
}
