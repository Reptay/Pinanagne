#include "typePanneau.hh"

int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande);
int getLargeur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande);
int min(int a, int b){
  if (a<b)
    return a;
  else
    return b;
}

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
  int minBande = 0;
  int hauteur =  getHauteur(imgRed, rayonFlou, cx, cy, minBande);
  int largeur = getLargeur(imgRed, rayonFlou, cx, cy, minBande);
  std::cout << "rayon " << r << std::endl;
  std::cout << "hauteur " << hauteur << std::endl;
  std::cout << "largeur " << largeur << std::endl;

  if (largeur == 0 || hauteur == 0 ||
      2*largeur < r || 2*hauteur < r ||
      2*r < largeur || 2*r < hauteur)
    return NULL;

  /* créé l'image du panneau uniquement */
  //cercle de l'image du panneau uniqueement
  Circle* c2 = new Circle(Point(largeur, hauteur),
			  min(hauteur, largeur));
  minBande += rayonFlou;

  Mat* panneau = new Mat(2*hauteur, 2*largeur,CV_8UC3, Scalar(0,0,0));
  for (int i = cx-largeur; i < cx+largeur; i++)
    for (int j = cy-hauteur; j < cy+hauteur; j++){

     Point3_<uchar>* pImg = img.ptr<Point3_<uchar> >(j,i);
     Point3_<uchar>* pPan = panneau->ptr<Point3_<uchar>>(j-cy+hauteur,i-cx+largeur);
     pPan->x = pImg->x;
     pPan->y = pImg->y;
     pPan->z = pImg->z;
   }

  /* Rend blanc l'exterieur du panneau, pour ne pas analyser le fond */
  Mat imgBlack = panneau->clone();
  for (int i = 0; i < imgBlack.cols; i++)
    for (int j = 0; j < imgBlack.rows; j++)
      {
	Point3_<uchar>* pImg = imgBlack.ptr<Point3_<uchar> >(j,i);
	int distCentre = sqrt(pow((j - c2->getCenter().x), 2) +
			      pow((i - c2->getCenter().y), 2));
	if (distCentre >= c2->getRadius() - minBande)
	  {
	    pImg->x = 255;
	    pImg->y = 255;
	    pImg->z = 255;
	  }

      }/*
  namedWindow("Display", WINDOW_AUTOSIZE);                                  
  imshow("Display", imgBlack);                                                   */
  waitKey(0);
   BlackFilter(imgBlack);

   return panneau;
}


/**
 * Retourne la moitié de la hauteur total du panneau + le rayonFlou
 */
int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande){
  int hauteur = 0;
  int findRed = 0;
  int bandeHaut = 0; //largeur de la partie rouge
  int bandeBas = 0;
  for(int i = cy; i < imgRed.rows; i++) // vers le bas
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(i,cx);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0){ // on est sur du rouge
        findRed = 1;
	bandeBas++;
      }
      else if (r != 0 && findRed <= rayonFlou && findRed != 0 &&
	       i < imgRed.rows-1){
        findRed++; // on est pas sur du rouge mais on en a deja vu (rayonFlou)
      }
      else if (findRed != 0){ //on est pas sur du rouge et on en a vu y'a trop longtemps
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
      if (r == 0){
        findRed = 1;
	bandeHaut++;
      }
      else if (r != 0 && findRed <= rayonFlou && findRed != 0){
        findRed++;
      }
      else if (findRed != 0){
	if (cy - i > hauteur)
	  hauteur = cy - i;
        break;
      }
    }
  if (bandeBas*2 < bandeHaut || bandeHaut*2 < bandeBas){
    std::cout << "Bande haut et bande bas disproportionné, typePanneau.cc"
	      <<std::endl;
    return 0;
  }
  if (hauteur / bandeBas < 3 || hauteur / bandeBas > 7 ||
      hauteur / bandeHaut < 3 || hauteur / bandeHaut > 7){
    std::cout <<
      "Hauteur disproportionné par rapport au contour, typePanneau.cc"<<
      std::endl;
    return 0;
  }
  minBande = bandeHaut;
  if (bandeBas < minBande)
    minBande = bandeBas;
  return hauteur;
}

/**
 * Fait la meme chose que getHauteur mais pour la largeur
 */
int getLargeur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande){
  int largeur = 0;
  int findRed = 0;
  int bandeDroite = 0;
  int bandeGauche = 0;

  for (int i = cx; i < imgRed.cols; i++) // vers la droite
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(cy,i);
      int r = p->z; // 0 = rouge, 255 = autre
      if (r == 0){
        findRed = 1;
	bandeDroite++;
      }
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
      if (r == 0){
        findRed = 1;
	bandeGauche++;
      }
      else if (r != 0 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
	if (cx -i > largeur)
	  largeur = cx - i;
	break;
      }
    }

  if (bandeDroite*2 < bandeGauche || bandeGauche*2 < bandeDroite){
    std::cout << "Bande gauche et bande droite disproportionné, typePanneau.cc"
              <<std::endl;
    return 0;
  }
  if (largeur / bandeDroite < 3 || largeur / bandeDroite > 7 ||
      largeur / bandeGauche < 3 || largeur / bandeGauche > 7){
    std::cout <<
      "largeur disproportionné par rapport au contour, typePanneau.cc"<<
      std::endl;
    return 0;
  }
  if (bandeGauche < minBande)
    minBande = bandeGauche;
  if (bandeDroite < minBande)
    minBande = bandeDroite;
  return largeur;
}


/**
 * Cherche les zones de la couleur=(r,g,b) EXACT
 */
/*
int nbCouleurContinu(Mat *img, int hauteur, int largeur, int rayonFLou
		     int r, int g, int b)
{
  int rayon;
  if (hauteur>largeur)
    rayon = hauteur - rayonFlou;
  else
    rayon = largeur - rayonFlou;
  int minBande = rayon/7; // correspond a la largeur de la bande minimal
  rayon = rayon-minBande; //rayon de recherche maximum (sans la bande rouge)
}
*/
