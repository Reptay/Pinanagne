#ifndef TYPEPANNEAU_HH
#define TYPEPANNEAU_HH

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <math.h>
#include <utility>
#include "circle.hh"
#include "../filter/filters.hh"

/**
 * Retourne l'image du panneau si c'est ok
 * Retourne NULL sinon
 */
struct sForme {
  int size;
  int hauteur;
  int largeur;
  int minx;
  int miny;
  int maxx;
  int maxy;
};

std::pair<Mat*, std::vector<sForme>>* isLimitation(Mat img, Circle* c);

#endif
