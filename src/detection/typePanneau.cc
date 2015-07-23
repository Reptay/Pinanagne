#include "typePanneau.hh"

int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy);
int getLargeur(Mat imgRed, int rayonFlou, int cx, int cy);

Mat* isLimitation(Mat img, Circle* c)
{
  if (c == NULL)
    return NULL;

  int r = c->getRadius();
  int cx = c->getCenter().x;
  int cy = c->getCenter().y;

  Mat imgRed = img.clone();
  RedFilter(imgRed);
  int rayonFlou = 1; // pour le calcul hauteur et largeur du cercle rouge
  int hauteur =  getHauteur(imgRed, rayonFlou, cx, cy);
  int largeur = getLargeur(imgRed, rayonFlou, cx, cy);
  std::cout << "rayon " << r << std::endl;
  std::cout << "hauteur " << hauteur << std::endl;
  std::cout << "largeur " << largeur << std::endl;

  if (largeur == 0 || hauteur == 0 ||
      2*largeur < r || 2*hauteur < r ||
      2*r < largeur || 2*r < hauteur)
    return NULL;

  /* créé l'image du panneau uniquement */
  Mat* panneau = new Mat(2*hauteur, 2*largeur,CV_8UC3, Scalar(0,0,0));
  for (int i = cx-largeur; i < cx+largeur; i++)
    for (int j = cy-hauteur; j < cy+hauteur; j++){
     Point3_<uchar>* pImg = img.ptr<Point3_<uchar> >(j,i);
     Point3_<uchar>* pPan = panneau->ptr<Point3_<uchar>>(j-cy+hauteur,i-cx+largeur);
     pPan->x = pImg->x;
     pPan->y = pImg->y;
     pPan->z = pImg->z;
   }

   return panneau;
}


int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy){
  int hauteur = 0;
  int findRed = 0;

  for(int i = cy; i < imgRed.rows; i++) // vers le bas
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(i,cx);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0)
        findRed = 1;
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
        hauteur = i - cy;
        break;
      }
    }
  if (hauteur == 0)
    return 0;
  findRed = 0;
  for (int i = cy; i >= 0; i--) // vers le haut
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(i,cx);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0)
        findRed = 1;
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
	if (cy - i > hauteur)
	  hauteur = cy - i;
        break;
      }

    }

  return hauteur;
}

int getLargeur(Mat imgRed, int rayonFlou, int cx, int cy){
  int largeur = 0;
  int findRed = 0;
  
  for (int i = cx; i < imgRed.cols; i++) // vers la droite
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(cy,i);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0)
        findRed = 1;
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
        largeur = i - cx;
        break;
      }
    }
  if (largeur == 0)
    return 0;
  findRed = 0;
  for (int i = cx; i >= 0; i--) // vers la gauche
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(cy,i);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0)
        findRed = 1;
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
	if (cx -i > largeur)
	  largeur = cx - i;
	break;
      }
    }

  return largeur;
}
