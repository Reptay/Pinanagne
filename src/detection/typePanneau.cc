#include "typePanneau.hh"

int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande);
int getLargeur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande);
std::vector<int> nbCouleurContinu(Mat *img, int r, int g, int b);
int min(int a, int b){
  if (a<b)
    return a;
  else
    return b;
}
double compare(int a, int b)
{
  double i = a;
  double j = b;
  if (i > j)
    return i / j;
  else
    return j / i;
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
  int rayonFlou = 2; // pour le calcul hauteur et largeur du cercle rouge
  int minBande = 0;
  int hauteur =  getHauteur(imgRed, rayonFlou, cx, cy, minBande);
  int largeur = getLargeur(imgRed, rayonFlou, cx, cy, minBande);
  /*
  std::cout << "rayon " << r << std::endl;
  std::cout << "hauteur " << hauteur << std::endl;
  std::cout << "largeur " << largeur << std::endl;
  */

  if (largeur == 0 || hauteur == 0 ||
      2*largeur < r || 2*hauteur < r ||
      2*r < largeur || 2*r < hauteur)
    {
      std::cerr<< "largeur ou hauteur non valide " << largeur
	       << " " << hauteur<<std::endl;
      return NULL;
    }

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

      }
   BlackFilter(imgBlack);
   /*   
   namedWindow("DisplayBlack", WINDOW_AUTOSIZE);
   imshow("DisplayBlack", imgBlack);   
   waitKey(0);
   */
   std::vector<int> sizes = nbCouleurContinu(&imgBlack,0,0,0);
   if (sizes.size() <= 1 || sizes.size() > 3){
     std::cerr << "typePanneau.cc Invalide nb de forme continu : " <<
       sizes.size() << std::endl;
     return NULL;
   }

   for (unsigned long i = 0; i < sizes.size()-1; i++)
     for (unsigned long j = 1; j < sizes.size(); j++)
       {
	 if (i == j)
	   continue;
	 double comp = compare(sizes[i], sizes[j]);
	 if (comp>2)
	   std::cerr << "typePanneau.cc Proportions des chiffres invalides"
		     << std::endl;
       }

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

int detecteZone(Mat *img, int i, int j, int r, int g, int b, int size,
		bool **visite);
std::vector<int> nbCouleurContinu(Mat *img, int r, int g, int b)
{
  bool** visite = new bool*[img->rows];
  for(int i = 0; i < img->rows; i++)
    visite[i] = new bool[img->cols];
  for (int i = 0; i < img->rows; i++)
    for (int j = 0; j < img->cols;j++){
      visite[i][j] = false;
    }

  std::vector<int> sizes;

  for (int i = 0; i < img->cols; i++)
    for (int j = 0; j < img->rows; j++)
      {
	Point3_<uchar>* pImg = img->ptr<Point3_<uchar> >(j,i);
	if (pImg->x == b && pImg->y == g && pImg->z == r && !visite[j][i])
	  {
	    sizes.push_back(detecteZone(img, i, j, r, g, b, 0, visite));
	  }
      }
  for(int i = 0; i < img->rows; i++)
    {
      delete [] visite[i];
    }
  delete [] visite;

  return sizes;
}

/**
 * explore une zone et retourne sa taille
 */
int detecteZone(Mat *img, int i, int j, int r, int g, int b, int size,
		bool **visite)
{
  visite[j][i] = true;
  size++;


  int rayonFlou = 2; // >= 1
  for (int x = rayonFlou*-1; x <= rayonFlou; x++)
    {
      if(x==0)
	continue;
      if (j+x < img->rows && !visite[j+x][i]){
	Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j+x,i);
	if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
	  size = detecteZone(img, i, j+x, r, g, b, size, visite);
      }
      if (j-x >= 0 && !visite[j-x][i]){
	Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j-x,i);
	if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
	  size = detecteZone(img, i, j-x, r, g, b, size, visite);
      }
      if (i+x < img->cols && !visite[j][i+x]){
	Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j,i+x);
	if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
	  size = detecteZone(img, i+x, j, r, g, b, size, visite);
      }
      if (i-x >= 0 && !visite[j][i-x]){
	Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j,i-x);
	if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
	  size = detecteZone(img, i-x, j, r, g, b, size, visite);
      }

    }

  /*
  if (j+1 < img->rows && !visite[j+1][i]){
    Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j+1,i);
    if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
      size = detecteZone(img, i, j+1, r, g, b, size, visite);
  }
  if (j-1 >= 0 && !visite[j-1][i]){
    Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j-1,i);
    if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
      size = detecteZone(img, i, j-1, r, g, b, size, visite);
  }
  if (i+1 < img->cols && !visite[j][i+1]){
    Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j,i+1);
    if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
      size = detecteZone(img, i+1, j, r, g, b, size, visite);
  }
  if (i-1 >= 0 && !visite[j][i-1]){
    Point3_<uchar>* pImg2 = img->ptr<Point3_<uchar> >(j,i-1);
    if (pImg2->x == b && pImg2->y == g && pImg2->z == r)
      size = detecteZone(img, i-1, j, r, g, b, size, visite);
  }
  */
  return size;
}
