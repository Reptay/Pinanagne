#include "test.hh"

Test::Test(std::vector<Panneau> p, std::vector<int> v)
  : panneaux_(p), vitesses_(v){}
std::vector<Panneau> Test::getPanneaux(){return panneaux_;}
std::vector<int> Test::getVitesses(){return vitesses_;}


Panneau ligneToPanneau(std::string ligne)
{
  std::string limitation = ligne.substr(0, ligne.find(" "));
  std::string time = ligne.substr(ligne.find(" ")+1, ligne.size()-1);
  std::string mn = time.substr(0, time.find(":"));
  std::string sec = time.substr(time.find(":")+1, time.size()-1);
  return Panneau(std::stoi(limitation),std::stol(mn),std::stoi(sec));
}

void panneauxToFile(std::vector<Panneau> panneaux)
{
  std::ofstream fichier("panTest.txt", std::ios::trunc);
  if (fichier)
    {
      for (std::vector<Panneau>::iterator it = panneaux.begin() ;
	   it != panneaux.end(); it++)
	{
	  fichier << (*it).print() << std::endl;
	}

      fichier.close();
    }
  else
    std::cerr << "test.cc panneauxToFile : Ouverture du fichier impossible" << std::endl;
}

void compareFilePanneaux(std::string pVideo, std::string pRef)
{
  std::ifstream fvideo(pVideo, std::ios::in);
  std::ifstream fref(pRef, std::ios::in);

  std::vector<Panneau> video;
  std::vector<Panneau> ref;

  if (fvideo && fref)
    {
      std::string ligne;
      while(getline(fvideo, ligne))
	{
	  video.push_back(ligneToPanneau(ligne));
	}
      while(getline(fref, ligne))
	ref.push_back(ligneToPanneau(ligne));

      fvideo.close();
      fref.close();
    }
  else{
    std::cerr << "Test.cc compareFilePanneaux : Impossible d'ouvrir le fichier !" << std::endl;
    return;
  }

  int panneauxBienDetectes=0;
  int limitationsBienLus=0;
  int fauxPositifs=0;

  int marge = 2; // en seconde
  std::vector<Panneau>::iterator itv = itv = video.begin();
  std::vector<Panneau>::iterator itr = ref.begin();

  // bonne detection et lecture de limitation
  while(itv != video.end())
    {
      long time = (*itv).getTime();
      int limitation = (*itv).getLimitation();
      if (abs(time-(*itr).getTime()) < marge){
	panneauxBienDetectes++;
	if (limitation == (*itr).getLimitation())
	  limitationsBienLus++;
	else
	  std::cout << "-- Erreur reconnaissance : " << (*itv).print() << std::endl;
	itr++;
      }
      else if ((*itr).getTime()<time){
	std::cout << "- Panneau non detecte : " << (*itr).print() << std::endl;
	itr++;
	if (itr == ref.end())
	  break;
	else
	  continue;
      }
      else
	{
	  std::cout << "--- Faux positifs : " <<  (*itv).print() << std::endl;
	}
      itv++;
    }

  fauxPositifs = video.size() - panneauxBienDetectes;

  std::cout << "Panneaux bien detectes : " <<panneauxBienDetectes<<"/"<<
    ref.size()<< " -> " << (float)panneauxBienDetectes/ref.size()*100<<"%"
	    << std::endl;
  std::cout << "Limitations bien lus : " << limitationsBienLus<<"/"<<
    panneauxBienDetectes<<" -> " << (float)limitationsBienLus/panneauxBienDetectes*100<<"%"<<std::endl;
  std::cout << "Faux positifs : " << fauxPositifs << " -> " <<
    (float)fauxPositifs/ref.size()*100<<"%"<<std::endl;
}

/************************************************
 * VITESSE
 ***********************************************/

int echantillonnnageRef(std::string pRef)
{
  std::ifstream fref(pRef, std::ios::in);

  int ech = 0;
  if (fref)
    {
      std::string ligne;

      if(getline(fref, ligne))
        ech = std::stoi(ligne);

      fref.close();
    }
  else{
    std::cerr << "Test.cc echantillonnnageRef : Impossible d'ouvrir le fichier !" << std::endl;
    return 0;
  }
  return ech;
}

void vitessesToFile(std::vector<int> vitesses)
{
  std::ofstream fichier("vitTest.txt", std::ios::trunc);
  if (fichier)
    {
      for (std::vector<int>::iterator it = vitesses.begin() ;
           it != vitesses.end(); it++)
        {
          fichier << (*it) << std::endl;
        }

      fichier.close();
    }
  else
    std::cerr << "test.cc panneauxToFile : Ouverture du fichier impossible" << \
      std::endl;
}

void compareFileVitesses(std::string pVideo, std::string pRef)
{
  std::ifstream fvideo(pVideo, std::ios::in);
  std::ifstream fref(pRef, std::ios::in);

  std::vector<int> video;
  std::vector<int> ref;

  if (fvideo && fref)
    {
      std::string ligne;
      while(getline(fvideo, ligne))
        {
          video.push_back(std::stoi(ligne));
        }
      while(getline(fref, ligne))
        ref.push_back(std::stoi(ligne));

      fvideo.close();
      fref.close();
    }
  else{
    std::cerr << "Test.cc compareFileVitesse : Impossible d'ouvrir le fichier \
!" << std::endl;
    return;
  }
  ref.erase(ref.begin()); // supprime la premiere ligne (frequence)

  int pos = 0;
  double diff = 0;
  while (pos < video.size() && pos < ref.size())
    {
      diff += abs(video[pos] - ref[pos]);
      pos++;
    }
  std::cout << "======= VITESSE =======" << std::endl;
  //std::cout << "diff " << " " << diff << std::endl;
  double moyDiff = diff/pos;
  std::cout << "Nombre de comparaison : " << pos << std::endl;
  std::cout << "Moyenne des erreurs : ";
  std::cout << moyDiff << "km/h"<< std::endl;
}

