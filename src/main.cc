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
#include "detection/panneau.hh"
#include <dirent.h>
#include "image_preprocessing/traitement.hh"
#include "test/test.hh"
#include "perspective/perspective-detector.hh"
#include "speed/snapshot.hh"

//info for perspective detector
#define FOCAL 0.031 //focal distance
#define SENSOR_SIZE FOCAL / 2.4 // Sensor size (width 0.0048, height 0.0036)
#define SENSOR_PXL 1534.4380078712857//Sensor size in pixel (2304x1536)

#define SCALE SENSOR_SIZE / SENSOR_PXL
int echantillonnageVitesses = 30; // en seconde


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

Test fluxWebcam(std::string path)
{
  std::vector<Snapshot> lines_old;
  std::vector<Snapshot> lines;
  std::vector<Panneau> panneauxDetectes;
  double est_speed = 0;
  std::vector<int> vitessesDetectes;
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
  //    vector<int> matches = vector<int>(7, 0);
  vector<Mat> save = vector<Mat>();
  vector<Mat> sav = vector<Mat>();
  vector<Mat> sa = vector<Mat>();
  vector<Mat> saving = vector<Mat>();
  // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
  double posVideo = -1;
  while(key != 'q' && key != 'Q') {
    posVideo++;
    // On récupère une image
    image = cvQueryFrame(capture);
    Mat img = cvarrToMat(image);
    if (img.empty())
      break;

    //if (posVideo<45500)
    // continue;
    //std::cout << posVideo<<std::endl;

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

    vector<int> vits = vector<int>(14);
    if (panneaux.size() != 0)
      {
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
        vits[vitmax / 10] += match;
        vit = traitementImage(sav);
        vits[(vit % 1000) / 10] += vit / 1000000;
        if (vitmax == vit % 1000)
          match += 10;
        if (match < vit / 1000000)
          {
            vitmax = vit % 1000;
            match = vit / 1000000;
          }
        vit = traitementImage(sa);
        vits[(vit % 1000) / 10] += vit / 1000000;
        if (vitmax == vit % 1000)
          match += 10;
        if (match < vit / 1000000)
          {
            vitmax = vit % 1000;
            match = vit / 1000000;
          }
        vit = traitementImage(saving);
        vits[(vit % 1000) / 10] += vit / 1000000;
        if (vitmax == vit % 1000)
          match += 10;
        if (match < vit / 1000000)
          {
            vitmax = vit % 1000;
            match = vit / 1000000;
          }
        int m = 0;
        for (int i = 0; i < 14; i++)
          {
            if (m < vits[i])
              {
                m = vits[i];
                vitmax = i * 10;
              }
          }
        if (vitmax && avitmax != vitmax)
          {
            cout << vitmax << endl;
            playLimitation(std::to_string(vitmax));
            avitmax = vitmax;
          }
        match = 0;
        double time=cvGetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC);//en ms
        panneauxDetectes.push_back(Panneau(vitmax, (long)round(time/1000)));
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

    /*for (std::vector<Mat>::iterator it = panneaux.begin();
      it != panneaux.end(); it++){
      namedWindow("Display", WINDOW_AUTOSIZE);
      imshow("Display", *it);
      }
    */
    lines_old = lines;
    lines= vector<Snapshot>();
    double time=cvGetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC);//en ms
    vector<RotatedRect> rects = getLines(img);
    Point2f rect_points[4];
    for (int i =0; i < rects.size(); i++)
      {
        rects[i].points(rect_points);
        for (int j = 0; j <4; j++)
          //Show rectangles
          {
            Point2f p1 = Point2f(rect_points[j].x, rect_points[j].y + img.rows / 2);
            Point2f p2 = Point2f(rect_points[(j+1)%4].x, rect_points[(j+1)%4].y + img.rows / 2);
            PerspectiveDetector p(1.5, FOCAL, SCALE);
            p.computePos(p1.x, p1.y, p2.x, p2.y);
            if (p.getDist() < 100)
              {
                line( img, p1, p2, Scalar(255, 0, 0), 1, 8);
                lines.emplace_back(p.getX(), p.getY(), p.getZ(), time / 1000);
              }
            else
              line( img, p1, p2, Scalar(0, 255, 255), 1, 8);
          }
      }
    int nbspeed = 0;
    double sum_speed = 0;
    for (auto l1 : lines_old)
      {
        //PerspectiveDetector p(1.5, FOCAL, SCALE);
        for (auto l2 : lines)
          {
            if (abs(l1.get_x() - l2.get_x()) <= 1
                && abs(l1.get_y() - l2.get_y()) <= 1
                && (l2 - l1) < 100 && (l2 - l1) > 0)
              {
                nbspeed++;
                sum_speed += Snapshot::mps_to_kph(l2 - l1);
              }
          }
      }
    est_speed = sum_speed / nbspeed;
    cerr << "speed : " << est_speed << endl;
    IplImage image2=img;
    cvShowImage( "Webcam", &image2);
    //waitKey(0);
    // On attend 10ms
    key = cvWaitKey(1);
    //  acu = acu % 2 + 1;
  }

  cvReleaseCapture(&capture);
  cvDestroyWindow("Webcam");
  //  exit(0);
  vitessesDetectes.push_back(50);
  vitessesDetectes.push_back(45);
  return Test(panneauxDetectes, vitessesDetectes);
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
  else if (argc == 2){ // fichier video
    Test test = fluxWebcam(argv[1]);
    panneauxToFile(test.getPanneaux());
    vitessesToFile(test.getVitesses());
  }
  else if ((argc == 4 || argc == 5) && strcmp(argv[1], "-t")==0) //execute et compare
    {
      if (argc == 5)
        {
          int echRef = echantillonnnageRef(argv[4]);
          if (echRef != 0)
            echantillonnageVitesses = echRef;
        }
      std::cout << "Periode echantillonage vitesses : " <<
        echantillonnageVitesses << " secondes"<< std::endl;
      Test test = fluxWebcam(argv[2]);
      panneauxToFile(test.getPanneaux());
      vitessesToFile(test.getVitesses());
      compareFilePanneaux("panTest.txt", argv[3]);//argv[3]);
      if (argc == 5)
        compareFileVitesses("vitTest.txt", argv[4]);
    }
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
            std::vector<Panneau> panneauxDetectes = fluxWebcam(filename).getPanneaux();
            if (panneauxDetectes.size()>0)
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
          vector<RotatedRect> rects = getLines(img);
          Point2f rect_points[4];
          for (int i =0; i < rects.size(); i++)
            {
              rects[i].points(rect_points);
              for (int j = 0; j <4; j++)
                //Show rectangles
                {
                  Point2f p1 = Point2f(rect_points[j].x, rect_points[j].y + img.rows / 2 - 1);
                  Point2f p2 = Point2f(rect_points[(j+1)%4].x, rect_points[(j+1)%4].y + img.rows / 2 - 1);
                  PerspectiveDetector p(1.5, FOCAL, SCALE);
                  p.computePos(p1.x, p1.y, p2.x, p2.y);
                  if (p.getDist() < 100)
                    line( img, p1, p2, Scalar(255, 0, 0), 1, 8);
                  else
                    line( img, p1, p2, Scalar(0, 255, 255), 1, 8);
                }

              Point2f p1 = Point2f(rect_points[1].x, rect_points[1].y + img.rows / 2 - 1);
              Point2f p2 = Point2f(rect_points[2].x, rect_points[2].y + img.rows / 2 - 1);
              PerspectiveDetector p(1.5, FOCAL, SCALE);
              p.computePos(p1.x, p1.y, p2.x, p2.y);
              cout << "rectangle #" << i << " distance si ligne blanche : " << p.getDist() <<endl;
              //if (p.getDist() < 100)
              //  line( img, p1, p2, Scalar(255, 255, 0), 1, 8);

            }
          imshow("test",img);
          waitKey();
          //    std::cout << getLines(img).size() << endl;
        }
    }
  else if (argc == 4 && strcmp(argv[1], "-c")==0) //compare 2 fichiers txt
    {
      compareFilePanneaux(argv[2], argv[3]);
    }
  else if (argc == 4 && strcmp(argv[1], "-cv")==0) // compare les vitesses
    {
      compareFileVitesses(argv[2], argv[3]);
      //              fichierVideo, fichier de reference
    }
  else
    std::cerr << "Invalid argument" << std::endl;

  Mix_CloseAudio();
}
