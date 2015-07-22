#include "typePanneau.hh"

int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy);

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
  std::cout << "hauteur " << hauteur << std::endl;
  // int largeur; TODO

  Mat* panneau = new Mat(2*r, 2*r,CV_8UC3, Scalar(0,0,0));
  for (int i = cx-r; i < cx+r; i++)
    for (int j = cy-r; j < cy+r; j++){
     Point3_<uchar>* pImg = img.ptr<Point3_<uchar> >(j,i);
     Point3_<uchar>* pPan = panneau->ptr<Point3_<uchar>>(j-cy+r,i-cx+r);

     pPan->x = pImg->x;
     pPan->y = pImg->y;
     pPan->z = pImg->z;
   }

   return panneau;
}

int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy){
  int hauteur = 0;
  int findRed = 0;
  
  for (int i = cx; i < imgRed.cols; i++)
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(cy,i);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0)
        findRed = 1;
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
        hauteur = i - cx;
        break;
      }
    }
  
  findRed = 0;
  for (int i = cx; i >= 0; i--)
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(cy,i);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0)
        findRed = 1;
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
	if (cx -i > hauteur)
	  hauteur = cx - i;
	break;
      }
    }
  return hauteur;
}
