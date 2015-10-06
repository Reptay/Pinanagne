#include "typePanneau.hh"

bool vrb = true; // Mode verbeux

int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande);
int getLargeur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande);
void supprimeFond(Mat *img, Circle* c);
struct sForme {
  int size;
  int hauteur;
  int largeur;
  int minx;
  int miny;
  int maxx;
  int maxy;
};
std::vector<sForme> nbCouleurContinu(Mat *img, int c);
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

  //  Mat imgRed = img.clone();
  Mat imgRed = RedFilter(img);
  int rayonFlou = 2; // pour le calcul hauteur et largeur du cercle rouge
  int minBande = 0;
  int hauteur =  getHauteur(imgRed, rayonFlou, cx, cy, minBande);
  if (hauteur == 0){
    if (vrb)
      std::cerr << "hauteur == 0" << std::endl;
    return NULL;
  }
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
      if (vrb)
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
   imgBlack = BlackFilter(imgBlack);

   std::vector<sForme> formes = nbCouleurContinu(&imgBlack,255);
   // supprime les petites zones (bruit)
   for (std::vector<sForme>::iterator it = formes.begin(); it!=formes.end();){
     if ((*it).size < largeur*0.5){
       if(vrb)
	 std::cerr << " typePanneau.cc : rm forme continu " << 
	   (*it).size << " " <<largeur<<std::endl;
       it = formes.erase(it);
     }
     else it++;
   }
   if (formes.size() <= 1 || formes.size() > 3){
     if (vrb)
       std::cerr << "typePanneau.cc Invalide nb de forme continu : " <<
	 formes.size() << std::endl;
     return NULL;
   }
   
   for (unsigned long i = 0; i < formes.size(); i++)
     if (compare(hauteur, formes[i].hauteur) > 2.3){
       if (vrb)
	 std::cerr << "typePanneau.cc Invalide Hauteur de forme continu : " <<
	   compare(hauteur, formes[i].hauteur) << std::endl;
       return NULL;
     }
   
   for (unsigned long i = 0; i < formes.size()-1; i++)
     for (unsigned long j = 1; j < formes.size(); j++)
       {
	 if (i == j)
	   continue;
	 double comp = compare(formes[i].size, formes[j].size);
	 double comp2 = compare(formes[i].hauteur, formes[j].hauteur);
	 if (comp>2 || comp2 > 1.5){
	   if (vrb)
	     std::cerr << "typePanneau.cc Proportions des chiffres invalides "
		       << formes[i].size <<" "<<formes[j].size<< std::endl;
	   return NULL;
	 }
       }
   /*for (std::vector<sForme>::iterator it = formes.begin();
	it!=formes.end();it++){
     std::cout << "-> " << (*it).hauteur << " " << (*it).largeur <<
       " " << hauteur << std::endl;
       }*/
   if (vrb)
     std::cout << "---> " << hauteur*2 << " "<<formes[0].hauteur << std::endl;

   supprimeFond(panneau, c2);
   return panneau;
}


/**
 * Retourne la moitié de la hauteur total du panneau + le rayonFlou
 */
int getHauteur(Mat imgRed, int rayonFlou, int cx, int cy, int &minBande){
  //  dp(imgRed);
  int hauteur = 0;
  int findRed = 0;
  int bandeHaut = 0; //largeur de la partie rouge
  int bandeBas = 0;

  for(int i = cy; i < imgRed.rows; i++) // vers le bas
    {
      Point3_<uchar>* p = imgRed.ptr<Point3_<uchar> >(i,cx);
      int r = p->z; // 255 = rouge, 0 = autre

      if (r == 255){ // on est sur du rouge
        findRed = 1;
	bandeBas++;
      }
      else if (r != 255 && findRed <= rayonFlou && findRed != 0 &&
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
      if (r == 255){
        findRed = 1;
	bandeHaut++;
      }
      else if (r != 255 && findRed <= rayonFlou && findRed != 0){
        findRed++;
      }
      else if (findRed != 0){
	if (cy - i > hauteur)
	  hauteur = cy - i;
        break;
      }
    }
  if (bandeBas == 0 || bandeHaut == 0){
    if (vrb)
      std::cerr << "Bande haut ou bas = 0" << std::endl;
    return 0;
  }
  if (bandeBas*2 < bandeHaut || bandeHaut*2 < bandeBas){
    if (vrb)
      std::cout << "Bande haut et bande bas disproportionné, typePanneau.cc "<<
	bandeHaut << " " << bandeBas <<std::endl;
    return 0;
  }
  if (hauteur / bandeBas < 1.5 || hauteur / bandeBas > 7 ||
      hauteur / bandeHaut < 1.5 || hauteur / bandeHaut > 7){
    if (vrb)
      std::cout <<
	"Hauteur disproportionné par rapport au contour, typePanneau.cc "<<
	hauteur << " " << bandeBas << " " << bandeHaut <<
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
      int r = p->z; // 255 = rouge, 0 = autre
      if (r == 255){
        findRed = 1;
	bandeDroite++;
      }
      else if (r != 255 && findRed <= rayonFlou && findRed != 0)
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
      int r = p->z;
      if (r == 255){
        findRed = 1;
	bandeGauche++;
      }
      else if (r != 255 && findRed <= rayonFlou && findRed != 0)
        findRed++;
      else if (findRed != 0){
	if (cx -i > largeur)
	  largeur = cx - i;
	break;
      }
    }

  if (bandeDroite == 0 || bandeGauche == 0){
    if (vrb)
      std::cerr << "Bande gauche ou droite = 0"<<std::endl;
    return 0;
  }
  if (bandeDroite*2 < bandeGauche || bandeGauche*2 < bandeDroite){
    if (vrb)
      std::cout<<"Bande gauche et bande droite disproportionné, typePanneau.cc"
		<<std::endl;
    return 0;
  }
  if (largeur / bandeDroite < 1.5 || largeur / bandeDroite > 10 ||
      largeur / bandeGauche < 1.5 || largeur / bandeGauche > 10){
    if (vrb)
      std::cout <<
	"largeur disproportionné par rapport au contour, typePanneau.cc "<<
	largeur << " " << bandeGauche<< " " << bandeDroite <<  std::endl;
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

int detecteZone(Mat *img, int i, int j,int c, int size,
		bool **visite, struct sForme *sf);
std::vector<sForme> nbCouleurContinu(Mat *img, int c)
{
  bool** visite = new bool*[img->rows];
  for(int i = 0; i < img->rows; i++)
    visite[i] = new bool[img->cols];
  for (int i = 0; i < img->rows; i++)
    for (int j = 0; j < img->cols;j++){
      visite[i][j] = false;
    }

  std::vector<sForme> formes;
 
  for (int i = 0; i < img->cols; i++)
    for (int j = 0; j < img->rows; j++)
      {
	int pImg = (int)img->at<uchar>(j,i);
	if (pImg == c && !visite[j][i])
	  {
	    sForme sf;
	    sf.minx = i; sf.maxx = i; sf.miny = j; sf.maxy = j;
	    sf.size = detecteZone(img, i, j, c, 0, visite, &sf);
	    sf.largeur = sf.maxx - sf.minx;
	    sf.hauteur = sf.maxy - sf.miny;
	    formes.push_back(sf);
	  }
      }

  for(int i = 0; i < img->rows; i++)
    {
      delete [] visite[i];
    }
  delete [] visite;
  return formes;
}

/**
 * explore une zone et retourne sa taille
 */
int detecteZone(Mat *img, int i, int j, int c, int size,
		bool **visite, struct sForme *sf)
{
  visite[j][i] = true;
  size++;
  // i largeur
  if (i < sf->minx)
    sf->minx = i;
  else if (i > sf->maxx)
    sf->maxx = i;
  if (j < sf->miny)
    sf->miny = j;
  else if (j > sf->maxy)
    sf->maxy = j;
  // j hauteur
  int rayonFlou = 2; // >= 1
  for (int x = rayonFlou*-1; x <= rayonFlou; x++)
    {
      if(x==0)
	continue;
      if (j+x < img->rows && j+x>=0 && !visite[j+x][i]){
	int pImg2 = (int)img->at<uchar>(j+x,i);
	if (pImg2 == c)
	  size = detecteZone(img, i, j+x, c, size, visite, sf);
      }
      if (j-x >= 0 && j-x < img->rows && !visite[j-x][i]){
	int pImg2 = (int)img->at<uchar>(j-x,i);
	if (pImg2 == c)
	  size = detecteZone(img, i, j-x, c, size, visite, sf);
      }
      if (i+x < img->cols && i+x >= 0 &&!visite[j][i+x]){
	int pImg2 = (int)img->at<uchar>(j,i+x);
	if (pImg2 == c)
	  size = detecteZone(img, i+x, j, c, size, visite, sf);
      }
      if (i-x >= 0 && i-x<img->cols && !visite[j][i-x]){
	int pImg2 = (int)img->at<uchar>(j,i-x);
	if (pImg2 == c)
	  size = detecteZone(img, i-x, j, c, size, visite, sf);
      }

    }
  return size;
}

void supprimeFond(Mat *img, Circle* c)
{
  for (int i = 0; i < img->cols; i++)
    for (int j = 0; j < img->rows; j++)
      {
	Point3_<uchar>* pImg = img->ptr<Point3_<uchar> >(j,i);
	int distCentre = sqrt(pow((j - c->getCenter().x), 2) +
			      pow((i - c->getCenter().y), 2));
	if (distCentre >= c->getRadius())
	  {
	    pImg->x = 255;
	    pImg->y = 255;
	    pImg->z = 255;
	  }
      }
}

int checkRect(Mat img)
{

}
