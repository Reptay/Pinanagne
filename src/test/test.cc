#include "test.hh"

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
  std::ofstream fichier("test.txt", std::ios::trunc);
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
    std::cerr << "test.cc : Ouverture du fichier impossible" << std::endl;
}

void compareFile(std::string pVideo, std::string pRef)
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
    std::cerr << "Test.cc : Impossible d'ouvrir le fichier !" << std::endl;
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
