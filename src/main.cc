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
		Mat img;
		img = imread(argv[2], CV_LOAD_IMAGE_COLOR);
		if (img.data)
		{
			traitementImage(argv[2]);
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
