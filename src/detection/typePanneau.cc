#include "typePanneau.hh"

Mat* isLimitation(Mat img, Circle* c)
{
  if (c == NULL)
    return NULL;

  int r = c->getRadius();
  int cx = c->getCenter().x;
  int cy = c->getCenter().y;

  //  int hauteur, largeur;



  Mat* panneau = new Mat(2*r, 2*r,CV_8UC3, Scalar(0,0,0));
   for (int i = cy-r; i < cy+r; i++)
     for (int j = cx-r; j < cx+r; j++){
     Point3_<uchar>* pImg = img.ptr<Point3_<uchar> >(i,j);
     Point3_<uchar>* pPan = panneau->ptr<Point3_<uchar>>(i-cy+r,j-cx+r);

     pPan->x = pImg->x;
     pPan->y = pImg->y;
     pPan->z = pImg->z;
   }
   return panneau;
}
