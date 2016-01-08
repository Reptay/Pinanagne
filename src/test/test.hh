#ifndef TEST_HH
#define TEST_HH

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../detection/panneau.hh"

class Test
{
public:
  Test(std::vector<Panneau> p, std::vector<int> v);
  std::vector<Panneau> getPanneaux();
  std::vector<int> getVitesses();
private:
  std::vector<Panneau> panneaux_;
  std::vector<int> vitesses_;
};

void panneauxToFile(std::vector<Panneau> panneaux);
void vitessesToFile(std::vector<int> vitesses);
int echantillonnnageRef(std::string pRef);
/**
 * pVideo : path du fichier qui contient les detections (créé par
 * la fonction panneauxToFile(std::vector<Panneau> panneaux))
 * pRef : le path du fichier de référence
 */
void compareFilePanneaux(std::string pVideo, std::string pRef);
void compareFileVitesses(std::string pVideo, std::string pRef);


#endif
