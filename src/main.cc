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
#include "image_preprocessing/traitement.hh"

using namespace std;

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
	int vitmax = 0;
int avitmax = 0;
	int vitzone = 0;
	int compt = 0;
	int match = 0;
	//vector<int> matchs = vector<int>(7, 0);
	vector<Mat> save = vector<Mat>();
	vector<Mat> sav = vector<Mat>();
	vector<Mat> sa = vector<Mat>();
	vector<Mat> saving = vector<Mat>();
	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q') {
		// On récupère une image
		image = cvQueryFrame(capture);
		Mat img = cvarrToMat(image);
		if (img.empty())
			break;

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


		if (panneaux.size() != 0)
		{
			panneauDetecte=true;
			compt = 0;
			saving = sa;
			sa = sav;
			sav = save;
			save = panneaux;
		}
		else if ( compt <= 20)
			compt++;
		if (save.size() != 0 && compt == 20)
		{
			int vit = traitementImage(save);
			match = vit / 1000000;
			vitmax = vit%1000;
			vit = traitementImage(sav);
			if (vitmax == vit % 1000)
				match += 10;
			if (match < vit / 1000000)
			{
				vitmax = vit % 1000;
				match = vit / 1000000;
			}

			vit = traitementImage(sa);
			if (vitmax == vit % 1000)
				match += 10;
			if (match < vit / 1000000)
			{
				vitmax = vit % 1000;
				match = vit / 1000000;
			}


			vit = traitementImage(saving);
			if (vitmax == vit % 1000)
				match += 10;
			if (match < vit / 1000000)
			{
				vitmax = vit % 1000;
				match = vit / 1000000;
			}

			if (vitmax && avitmax != vitmax)
			{
				cout << vitmax << endl;
				playLimitation(std::to_string(vitmax));
				avitmax = vitmax;
			}
			compt = 0;
		}
		/*if (vitzone != vit / 1000)
		  vitzone = vit / 1000;
		  if (vit % 1000 != 0)
		  {
		  matchs[((vit%1000)/10)%7]++;
		  compt = 0;
		  }
		  if (compt > 7)
		  {
		  int m = matchs[0];
		  int v = 0;
		  for (int i = 1; i < matchs.size(); i++)

		  if (m < matchs[i])


		  {
		  m = matchs[i];
		  v = i;
		  }
		  if (v == 3 || v == 5)
		  vitmax = v * 10;
		  else
		  vitmax = (v + 7) *10;

		  playLimitation(std::to_string(vitmax));		
		  matchs.clear();
		  matchs = vector<int>(7,0);
		  compt = 0; 
		  }*/

		/*		  for (std::vector<Mat>::iterator it = panneaux.begin();
				  it != panneaux.end(); it++){
				  namedWindow("Display", WINDOW_AUTOSIZE);
				  imshow("Display", *it);
				  }

				  IplImage image2=img;
				  cvShowImage( "Webcam", &image2);
		//waitKey(0); 
		// On attend 10ms
		key = cvWaitKey(1);*/
		//	acu = acu % 2 + 1;
	}

	//cvReleaseCapture(&capture);
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

int main(int argc, char* argv[])
{ 
	SDL_Init(SDL_INIT_AUDIO);

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
		int vitmax = 0;
		int vitzone = 0;
		Mat img;
		img = imread(argv[2], CV_LOAD_IMAGE_COLOR);
		if (img.data)
		{
			int vit = traitementImage(img);
			if (vit % 1000 != vitmax)
			{
				vitmax = vit % 1000;
				playLimitation(std::to_string(vitmax));
			}
			if (vit / 1000 != vitzone)
				vitzone = vit / 1000;
			//Comparaison avec les modeles via un surf
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
