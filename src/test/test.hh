#ifndef TEST_HH
#define TEST_HH

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../detection/panneau.hh"

void panneauxToFile(std::vector<Panneau> panneaux);

/**
 * pVideo : path du fichier qui contient les detections (créé par
 * la fonction panneauxToFile(std::vector<Panneau> panneaux))
 * pRef : le path du fichier de référence
 */
void compareFile(std::string pVideo, std::string pRef);

#endif
