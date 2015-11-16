#ifndef TRAITEMENT_HH
#define TRAITEMENT_HH

#include <cmath>
#include <string>
#include <vector>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include "../filter/filters.hh"
#include "../detection/shape.hh"
#include "../detection/typePanneau.hh"
#include "../ransac/ransac.hh"
#include "../surf/surf.hh"
#include "../detection/detectRect.hh"
#include <dirent.h>

int traitementImage(char* path);

#endif
