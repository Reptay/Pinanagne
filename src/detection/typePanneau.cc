#include "typePanneau.hh"

Mat* isLimitation(Mat img, Circle* c)
{
  if (c == NULL)
    return NULL;

  int r = c->getRadius();
  int cx = c->getCenter().x;
  int cy = c->getCenter().y;

  //int hauteur, largeur;




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
